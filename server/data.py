# coding=utf-8

import urllib.request
import urllib.parse
import os
import json
import datetime
import sys
import csv
import xml.etree.ElementTree
import math
import numbers


import lxml.html 

def assert_valid_windspeed(w):
    assert isinstance(w, numbers.Number), "Windspeed not a number: {}".format(type(w))
    assert 0.0 < w < 40.0, "Invalid {}".format(w)
    return  

def windspeed_ukenergy():
    url = 'https://ukenergy.statoil.com/wind'
    d = urllib.request.urlopen(url).read()
    root = lxml.html.fromstring(d)

    # data-windspeed is in meters/second
    # speed-label is shown by JS as miles-per-hours (at least with en_US locale)
    data_windspeed = root.xpath("//div[@id='wind-widget']/@data-windspeed")
    assert len(data_windspeed) == 1, 'Could not find element with data-windspeed'
    windspeed = float(data_windspeed[0])

    assert_valid_windspeed(windspeed)
    return windspeed

def main():
    wind_speed = windspeed_ukenergy()
    print('w', wind_speed)

if __name__ == '__main__':
    main()

