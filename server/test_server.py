
import data

def test_fetch_windspeed():

    wind = data.windspeed_ukenergy()
    assert 1.0 < wind < 25.0
