#ifndef _SPP_H_
#define _SPP_H_

#include <utility> // std::pair
#include <climits> // ulonglong
#include <vector> // std::vector
#include <limits> // std::numeric_limits
#include "graph.h" // types, graph::Graph
#include "timer.h" // timer

namespace spp{
  using namespace graph;
  using dist_t = long long;
  using distances_t = std::vector<dist_t>;
  using parents_t = std::vector<std::vector<node_t>>;
  constexpr dist_t INF = std::numeric_limits<dist_t>::max();
  
  extern double lastTiming;

  Graph spp_dag(parents_t &p);
  Graph spp_tree(parents_t &p);
  void get_orders_tree(const Graph &tree, const std::vector<bool> &vis, std::vector<node_t> &pos_order, int u);
  GraphW spp_fs_auction(const GraphW &g, distances_t &d);

  template<typename queue_t>
  void dijkstra_parents(const GraphW &g, node_t s, distances_t &d, parents_t &p, queue_t &q){
    q.clear();
    d.assign(g.n_nodes() + 1, spp::INF);
    p.resize(g.n_nodes() + 1);
    for(auto &v : p) v.clear();

    { //timing scope
      timer t(&lastTiming);

      q.insert(d[s]=0, s);
      p[s].resize(1), p[s][0] = s; //root
      while(!q.empty()){
        auto [du, u] = q.extract_min();

        for(auto [v, w] : g[u]){
          auto d2 = du + w;
          if(d2 < d[v]){
            p[v].resize(1), p[v][0] = u;
            if(d[v]==INF) q.insert(d[v] = du+w, v);
            else q.decrease_key(d[v] = du+w, v);
          }
          else if(d2 == d[v]) p[v].push_back(u);
        }
      }
    }
  }

  template<typename queue_t>
  void dijkstra(const GraphW &g, node_t s, distances_t &d, queue_t &q){
    q.clear();
    d.assign(g.n_nodes() + 1, spp::INF);

    { //timing scope
      timer t(&lastTiming);

      q.insert(d[s]=0, s); //root
      while(!q.empty()){
        auto [du, u] = q.extract_min();
        for(auto [v, w] : g[u]){
          if(d[v] > du + w){
            if(d[v]==INF) q.insert(d[v] = du+w, v);
            else q.decrease_key(d[v] = du+w, v);
          }
        }
      }
    }
  }

  template<typename queue_t> // r is previous source
  void r_dijkstra_dag(const GraphW &g, node_t s, distances_t &ds, const distances_t &dr, const Graph &dagr, queue_t &q){
    static std::vector<node_t> done;

    q.clear();
    ds.assign(g.n_nodes() + 1, spp::INF);
    done.resize(g.n_nodes() + 1);

    { //timing scope
      timer t(&lastTiming);

      //reoptimization
      size_t doneSz=1;
      done[0] = s; ds[s] = 0; //root
      for(size_t i=0;i<doneSz;++i){
        for(auto v : dagr[done[i]]){
          if(ds[v]!=INF) continue; //already done (reoptimization)
          ds[v] = dr[v] - dr[s];
          done[doneSz++] = v;
        }
      }

      q.build(ds, done.begin(), done.begin() + doneSz);
      //==============

      while(!q.empty()){
        auto [du, u] = q.extract_min();
        for(auto [v, w] : g[u]){
          if(ds[v] > du + w){
            if(ds[v]==INF) q.insert(ds[v] = du+w, v);
            else q.decrease_key(ds[v] = du+w, v);
          }
        }
      }
    }
  }

  template<typename queue_t> // r is previous source
  void r_dijkstra_tree(const GraphW &g, node_t s, distances_t &ds, const distances_t &dr, const Graph &tr, queue_t &q){
    static std::vector<node_t> done;

    q.clear();
    ds.assign(g.n_nodes() + 1, spp::INF);
    done.resize(g.n_nodes() + 1);

    { //timing scope
      timer t(&lastTiming);

      //reoptimization
      size_t doneSz=1;
      done[0] = s; ds[s] = 0; //root
      for(size_t i=0;i<doneSz;++i){
        for(auto v : tr[done[i]]){
          ds[v] = dr[v] - dr[s];
          done[doneSz++] = v;
        }
      }

      q.build(ds, done.begin(), done.begin() + doneSz);
      //==============

      while(!q.empty()){
        auto [du, u] = q.extract_min();
        for(auto [v, w] : g[u]){
          if(ds[v] > du + w){
            if(ds[v]==INF) q.insert(ds[v] = du+w, v);
            else q.decrease_key(ds[v] = du+w, v);
          }
        }
      }
    }
  }

  template<typename queue_t> // r is previous source
  void r_auction_dag(const GraphW &fs, node_t s, distances_t &ds, const distances_t &dr, const Graph &dagr, queue_t &q){
    const node_t n = fs.n_nodes();

    // preliminar 1
    std::vector<long long> price(n+1), w(n+1);
    std::vector<bool> vis(n+1);

    std::vector<long long> best_bid(n+1);
    std::vector<std::vector<std::tuple<long long, node_t, long long>>> bid(n+1); // ps + w

    std::vector<node_t> done(n+1);

    ds.assign(n + 1, spp::INF);
    q.clear();
    
    auto create_fs_bids = [&](node_t u) {
      for(auto [v, c] : fs[u])
        if(!vis[v])
          bid[u].emplace_back(c + price[v], v, c);
      sort(bid.begin(), bid.end());
      price[u] = (!bid[u].empty() ? get<0>(bid[u][0]) : INF);
    };

    auto erase_bs = [&](node_t u) { vis[u] = true; };
    auto update_best_bid = [&](node_t u) {
      for(long long &it = best_bid[u]; it < (long long) bid[u].size(); ++it) {
        node_t v = get<1>(bid[u][it]);
        if(!vis[v]) return;
      }
    };

    auto has_child = [&](node_t u) {
      update_best_bid(u);
      return best_bid[u] < (long long)bid[u].size();
    };

    auto add_subtree_to_answer = [&](node_t root) {
      size_t doneSz=1;

      done[0] = root;
      for(size_t i=0;i<doneSz;++i){
        for(auto v : dagr[done[i]]){
          if(vis[v]) continue;
          done[doneSz++] = v;
          erase_bs(v);
        }
      }

      for(size_t i=1;i<doneSz;++i){ //skip root
        node_t u = done[i];
        ds[u] = ds[root] + (dr[u] - dr[root]);
        // ps[u] = pr[u];
        create_fs_bids(u);
        w[u] = w[root] + price[u];
        if(has_child(u)) q.insert(w[u],u);
      }

      if(has_child(root)) q.insert(w[root],root);
    };

    { //timing scope
      timer t(&lastTiming);
      ds[s] = 0;
      create_fs_bids(s);
      w[s] = price[s];
      erase_bs(s);
      add_subtree_to_answer(s);

      while(!q.empty()) {
        auto [du, u] = q.extract_min();
        if(!has_child(u) || get<0>(bid[u][best_bid[u]]) >= INF) { // (a)
          price[u] = w[u] = INF;
          continue;
        }
        
        const auto &[mn_bid, v, cuv] = bid[u][best_bid[u]];
        if(price[u] < mn_bid) { // (b)
          long long old_price = price[u];
          price[u] = mn_bid;
          w[u] = w[u] + price[u] - old_price;
        } else { // (c)
          // ps[v] = i;
          ds[v] = ds[u] + cuv - (dr[u] - dr[v]);

          erase_bs(v);
          create_fs_bids(v);
          w[v] = w[u] + price[v];
          add_subtree_to_answer(v);
        }
        q.insert(w[u],u);
      }
      // swap(ds, dr);
      // swap(ps, pr);
    }
  }

  template<typename queue_t> // r is previous source
  void r_auction_tree(const GraphW &fs, node_t s, distances_t &ds, const distances_t &dr, const Graph &tr, queue_t &q){
    const node_t n = fs.n_nodes();

    // preliminar 1
    std::vector<long long> price(n+1), w(n+1);
    std::vector<bool> vis(n+1);

    std::vector<long long> best_bid(n+1);
    std::vector<std::vector<std::tuple<long long, node_t, long long>>> bid(n+1); // ps + w

    ds.assign(n + 1, spp::INF);
    q.clear();
    
    auto create_fs_bids = [&](node_t u) {
      for(auto [v, c] : fs[u])
        if(!vis[v])
          bid[u].emplace_back(c + price[v], v, c);
      sort(bid.begin(), bid.end());
      price[u] = (!bid[u].empty() ? get<0>(bid[u][0]) : INF);
    };

    auto erase_bs = [&](node_t u) { vis[u] = true; };
    auto update_best_bid = [&](node_t u) {
      for(long long &it = best_bid[u]; it < (long long) bid[u].size(); ++it) {
        node_t v = get<1>(bid[u][it]);
        if(!vis[v]) return;
      }
    };

    auto has_child = [&](node_t u) {
      update_best_bid(u);
      return best_bid[u] < (long long)bid[u].size();
    };

    auto add_subtree_to_answer = [&](node_t root) {
      std::vector<node_t> pos_order;
      get_orders_tree(tr, vis, pos_order, root);
      pos_order.pop_back(); // remove root
      
      for(node_t u : pos_order) erase_bs(u);

      for(node_t u : pos_order) {
        ds[u] = ds[root] + (dr[u] - dr[root]);
        // ps[u] = pr[u];
        create_fs_bids(u);
        w[u] = w[root] + price[u];
        if(has_child(u)) q.insert(w[u],u);
      }

      if(has_child(root)) q.insert(w[root],root);
    };

    { //timing scope
      timer t(&lastTiming);
      ds[s] = 0;
      create_fs_bids(s);
      w[s] = price[s];
      erase_bs(s);
      add_subtree_to_answer(s);

      while(!q.empty()) {
        auto [du, u] = q.extract_min();
        if(!has_child(u) || get<0>(bid[u][best_bid[u]]) >= INF) { // (a)
          price[u] = w[u] = INF;
          continue;
        }
        
        const auto &[mn_bid, v, cuv] = bid[u][best_bid[u]];
        if(price[u] < mn_bid) { // (b)
          long long old_price = price[u];
          price[u] = mn_bid;
          w[u] = w[u] + price[u] - old_price;
        } else { // (c)
          // ps[v] = i;
          ds[v] = ds[u] + cuv - (dr[u] - dr[v]);

          erase_bs(v);
          create_fs_bids(v);
          w[v] = w[u] + price[v];
          add_subtree_to_answer(v);
        }
        q.insert(w[u],u);
      }
      // swap(ds, dr);
      // swap(ps, pr);
    }
  }

}

#endif