import pandas as pd
from os import listdir


def summary_file(file_name):
    graph_instance = file_name.split('-')[0]
    instance_data = pd.read_csv(file_name)
    instance_data = instance_data.drop(columns='new_source')#I kept the column new source in the csv for a possible debug of an anomalous case, but here they are useless
    
    by_src_and_group = instance_data.groupby(['source', 'group']).mean()#avg of the groups of each source
    
    result = pd.DataFrame(by_src_and_group.groupby('group').mean())#now the avg of each group
    
    result['instance'] = graph_instance
    print(result)
    return result







# csv_files = ['planar-example.csv', 'grid-example.csv'] #here we need  the list of all files to be processed... I'm assuming that the graph type is the first thing before the '-'
csv_files = [filename for filename in listdir('./') if filename.endswith('.csv')]
print(csv_files)
result = pd.DataFrame()

for file in csv_files:
    result = pd.concat([result, summary_file(file)])

result.to_csv('out/output.csv')
