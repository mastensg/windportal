
import wp_gust

State, Inputs = wp_gust.State, wp_gust.Inputs

def test_gust():
    # 
    s = wp_gust.next_state(State(), Inputs(perturbation_amplitude=0.0, wind_speed=40.0, scale=1.0))
    assert s.__dict__['fan_duty'] == 1.0
