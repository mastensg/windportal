
import gust_gui

State, Inputs = gust_gui.State, gust_gui.Inputs

def test_gust():
    # 
    s = gust_gui.next_state(State(), Inputs(perturbation_amplitude=0.0, wind_speed=40.0, scale=1.0))
    assert s.__dict__['fan_duty'] == 1.0
