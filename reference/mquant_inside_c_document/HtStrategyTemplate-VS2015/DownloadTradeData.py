# encoding: utf-8

from mquant_api import *
from mquant_struct import *
import json
import pandas as pd
import datetime
from pandas.io.formats.csvs import CSVFormatter
def download_trade_data(param):
    """
    下载交易数据
    """
    print('enter download_trade_data:',param)
    trade_data_tmp = Trade()
    trade_list_dict={}
    trade_list = get_trades(only_this_inst=False)
    print('trade list:', trade_list)
    for trade_data in trade_list.values():
        for key in trade_data.__dict__.keys():
            if trade_list_dict.get(key) is None:
                trade_list_dict[key] = []
            trade_list_dict[key].append(trade_data.__dict__[key])
#    df_trade.to_csv('trade_' + datetime.datetime().now().strftime('%H:%M:%S') + '.csv',index=False)
    df_trade = pd.DataFrame.from_dict(trade_list_dict)
    df_trade.to_csv('D:\\trade.csv',index=False)
    print('end download_trade_data:',param)
    
    return "download success"