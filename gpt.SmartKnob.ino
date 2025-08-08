#include "configuration.h"
#include "display_task.h"
#include "interface_task.h"
#include "motor_task.h"

// Instantiate configuration
Configuration config;

// Handle display task pointer depending on SK_DISPLAY flag
#if SK_DISPLAY
DisplayTask display_task(0);
DisplayTask* display_task_p = &display_task;
#else
DisplayTask* display_task_p = nullptr;
#endif

MotorTask motor_task(1, config);
InterfaceTask interface_task(0, motor_task, display_task_p);

void setup() {
  #if SK_DISPLAY
  display_task.setLogger(&interface_task);
  display_task.begin();
  motor_task.addListener(display_task.getKnobStateQueue());
  #endif

  interface_task.begin();
  config.setLogger(&interface_task);
  config.loadFromDisk();
  interface_task.setConfiguration(&config);
  motor_task.setLogger(&interface_task);
  motor_task.begin();

  // Stop built-in loop task
  vTaskDelete(NULL);
}

void loop() {
  // Empty—FreeRTOS handles everything
}
