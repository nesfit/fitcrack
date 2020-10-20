/*
 * Author : Dávid Bolvanský
 * Licence: MIT, see LICENSE
 */

#include <fstream>
#include <iostream>
#include <map>
#include <string>

int main(int argc, char **argv) {
  if (argc != 2)
    return 1;
  std::ios_base::sync_with_stdio(false);
  std::string dict = argv[1];
  std::map<unsigned, unsigned> pwd_dist;

  std::ifstream file(dict);
  if (!file.is_open())
    return 1;

  std::string pwd;
  unsigned pwd_len;
  while (getline(file, pwd)) {
    pwd_len = pwd.length();
    if (pwd_len)
      pwd_dist[pwd_len] += 1;
  }
  file.close();

  for (std::map<unsigned, unsigned>::iterator it = pwd_dist.begin();
       it != pwd_dist.end(); it++)
    std::cout << it->first << ':' << it->second << ';';

  return 0;
}