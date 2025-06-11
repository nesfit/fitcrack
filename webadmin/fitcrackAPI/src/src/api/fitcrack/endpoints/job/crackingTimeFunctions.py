'''
   * Author : Lucie Jadrná and authors listed in AUTHORS
   * Licence: MIT, see LICENSE
'''


import math
import datetime

def adaptive_scheduling(tmin, tmax, sr, powers, distribution_coefficient_alpha, ramp_down_coefficient):
    """
    Calculates desired time for workunit based on given parameters.

    Parameters:
    - tmin: min workunit time
    - tmax: max workunit time
    - sr: total keyspace
    - powers: list of powers of connected hosts
    - distribution_coefficient_alpha: distribution coefficient
    - ramp_down_coefficient: ramp down coefficient

    Returns:
    - tp: desired workunit time
    """
    vSum = sum(powers)
    tp = (sr / vSum) * float(distribution_coefficient_alpha)
    tprealmin = max(tmin, tmax * float(ramp_down_coefficient))
    if tp < tprealmin:
        tp = tprealmin
    else:
        tp = min(tp, tmax)

    return tp


def compute_hostkeyspace_from_tp(tp, powers, total_keyspace):
    """
    Calculate host keyspaces based on tp, power of host, and total keyspace.

    Parameters:
    - tp: desired time for workunit
    - powers: list of powers of hosts
    - total_keyspace: total keyspace

    Returns:
    - host_keyspaces: list of lists represented as [keyspace, power, num_wu] for each host
    """
    host_keyspaces = []
    for power in powers:
        host_keyspaces.append([min(tp * power, total_keyspace), power, 1]) 
    return host_keyspaces


def which_node_will_compute(host_keyspaces, total_keyspace):
    """
    Determine which nodes will compute based on their keyspaces and the total keyspace.

    Parameters:
    - host_keyspaces: List of lists represented as [keyspace, power, num_wu] for all connected nodes
    - total_keyspace: total keyspace

    Returns:
    - keyspaces_of_computing_nodes: list of nodes that will compute, each represented as [keyspace, power, num_wu]
    """
    keyspaces_of_computing_nodes = []

    sum_host_keyspaces = sum(keyspace[0] for keyspace in host_keyspaces)
    
    if sum_host_keyspaces > total_keyspace:

        if max(keyspace[0] for keyspace in host_keyspaces) >= total_keyspace:
            for sublist in host_keyspaces:
                if sublist[0] == max(keyspace[0] for keyspace in host_keyspaces):
                    keyspaces_of_computing_nodes.append(sublist)
                    return keyspaces_of_computing_nodes

        while host_keyspaces and (total_keyspace - max(keyspace[0] for keyspace in host_keyspaces) > 0):
            x = max(keyspace[0] for keyspace in host_keyspaces)  
            for sublist in host_keyspaces:      
                if sublist[0] == x:
                    keyspaces_of_computing_nodes.append(sublist)
                    host_keyspaces.remove(sublist)
                    total_keyspace = total_keyspace - x
                    y = max(keyspace[0] for keyspace in host_keyspaces)
                    if total_keyspace - y <= 0:
                        for sublist in host_keyspaces:
                            if sublist[0] == y:
                                keyspaces_of_computing_nodes.append([total_keyspace, sublist[1], 1])       
                                return keyspaces_of_computing_nodes
                    break  
    else:
        keyspaces_of_computing_nodes = host_keyspaces
        cut_total_keyspace = total_keyspace - sum(keyspace[0] for keyspace in host_keyspaces)
        sum_keyspaces = sum(keyspace[0] for keyspace in host_keyspaces)
        
        while cut_total_keyspace - sum_keyspaces >= 0:
            modified_list = []

            for i, sublist in enumerate(keyspaces_of_computing_nodes):
                modified_list.append([sublist[0] + host_keyspaces[i][0], sublist[1], sublist[2]+1])

            keyspaces_of_computing_nodes = modified_list
            cut_total_keyspace = cut_total_keyspace - sum_keyspaces
            
        
        if cut_total_keyspace - sum_keyspaces < 0:
            while host_keyspaces and (cut_total_keyspace - max(keyspace[0] for keyspace in host_keyspaces) >= 0):
                x = max(keyspace[0] for keyspace in host_keyspaces)     # largest keyspace
                
                for i,sublist in enumerate(host_keyspaces):      
                    if sublist[0] == x:
                        if cut_total_keyspace - x <= 0:
                            keyspaces_of_computing_nodes[i] = [keyspaces_of_computing_nodes[i][0] + cut_total_keyspace, keyspaces_of_computing_nodes[i][1], keyspaces_of_computing_nodes[i][2]+1]
                            return keyspaces_of_computing_nodes
                        else:
                            host_keyspaces.remove(sublist)
                            keyspaces_of_computing_nodes[i] = [keyspaces_of_computing_nodes[i][0] + x, keyspaces_of_computing_nodes[i][1], keyspaces_of_computing_nodes[i][2]+1]
                            cut_total_keyspace = cut_total_keyspace - x
                        break
            if cut_total_keyspace - max(keyspace[0] for keyspace in host_keyspaces) < 0:
                x = max(keyspace[0] for keyspace in host_keyspaces) 
                for i,sublist in enumerate(host_keyspaces):
                    if sublist[0] == x:
                        keyspaces_of_computing_nodes[i] = [keyspaces_of_computing_nodes[i][0] + cut_total_keyspace, keyspaces_of_computing_nodes[i][1], keyspaces_of_computing_nodes[i][2]+1]
                        break
        
    return keyspaces_of_computing_nodes


def compute_estimate_forall(list_key_power_wu, wu_overhead):
    """
    Calculates the estimated time for all computing hosts and returns the maximum.

    Parameters:
    - list_key_power_wu: list of lists containing keyspaces, powers, and number of workunits
    - wu_overhead: Overhead time for one workunit

    Returns:
    - Maximum estimated time among all computing hosts
    """
    list_times = []
    for list in list_key_power_wu:
        list_times.append((list[0] / list[1]) + (list[2] + 1) * wu_overhead + 34)
    
    return max(list_times)
