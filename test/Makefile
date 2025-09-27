CXX = g++
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -lm

TESTS = test_navigation test_safety_logic test_button_logic test_tack_logic test_led_patterns test_course_adjustment test_state_machine test_servo_control test_sensitivity_adjustment test_settings_persistence test_watchdog_safety test_button_debounce test_sensor_backoff

all: $(TESTS)
	@./test_navigation
	@./test_safety_logic
	@./test_button_logic
	@./test_tack_logic
	@./test_led_patterns
	@./test_course_adjustment
	@./test_state_machine
	@./test_servo_control
	@./test_sensitivity_adjustment
	@./test_settings_persistence
	@./test_watchdog_safety
	@./test_button_debounce
	@./test_sensor_backoff

test_navigation: test_navigation.cpp
	$(CXX) $(CXXFLAGS) -o test_navigation test_navigation.cpp $(LDFLAGS)

test_safety_logic: test_safety_logic.cpp
	$(CXX) $(CXXFLAGS) -o test_safety_logic test_safety_logic.cpp $(LDFLAGS)

test_button_logic: test_button_logic.cpp
	$(CXX) $(CXXFLAGS) -o test_button_logic test_button_logic.cpp $(LDFLAGS)

test_tack_logic: test_tack_logic.cpp
	$(CXX) $(CXXFLAGS) -o test_tack_logic test_tack_logic.cpp $(LDFLAGS)

test_led_patterns: test_led_patterns.cpp
	$(CXX) $(CXXFLAGS) -o test_led_patterns test_led_patterns.cpp $(LDFLAGS)

test_course_adjustment: test_course_adjustment.cpp
	$(CXX) $(CXXFLAGS) -o test_course_adjustment test_course_adjustment.cpp $(LDFLAGS)

test_state_machine: test_state_machine.cpp
	$(CXX) $(CXXFLAGS) -o test_state_machine test_state_machine.cpp $(LDFLAGS)

test_servo_control: test_servo_control.cpp
	$(CXX) $(CXXFLAGS) -o test_servo_control test_servo_control.cpp $(LDFLAGS)

test_sensitivity_adjustment: test_sensitivity_adjustment.cpp
	$(CXX) $(CXXFLAGS) -o test_sensitivity_adjustment test_sensitivity_adjustment.cpp $(LDFLAGS)

test_settings_persistence: test_settings_persistence.cpp
	$(CXX) $(CXXFLAGS) -o test_settings_persistence test_settings_persistence.cpp $(LDFLAGS)

test_watchdog_safety: test_watchdog_safety.cpp
	$(CXX) $(CXXFLAGS) -o test_watchdog_safety test_watchdog_safety.cpp $(LDFLAGS)

test_button_debounce: test_button_debounce.cpp
	$(CXX) $(CXXFLAGS) -o test_button_debounce test_button_debounce.cpp $(LDFLAGS)

test_sensor_backoff: test_sensor_backoff.cpp
	$(CXX) $(CXXFLAGS) -o test_sensor_backoff test_sensor_backoff.cpp $(LDFLAGS)

clean:
	rm -f $(TESTS)

.PHONY: all clean