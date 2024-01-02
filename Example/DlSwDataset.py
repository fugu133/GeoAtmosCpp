# https://celestrak.org/SpaceData/SW-Last5Years.csv からダウンロード

import requests

url = 'https://celestrak.org/SpaceData/SW-Last5Years.csv'
file_name='SW-Last5Years.csv'

url_data = requests.get(url).content

with open(file_name, mode='wb') as f:
  f.write(url_data)