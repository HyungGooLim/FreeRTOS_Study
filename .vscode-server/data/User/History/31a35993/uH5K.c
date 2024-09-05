#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

// Task 핸들러 선언
TaskHandle_t Task1Handle = NULL;
TaskHandle_t Task2Handle = NULL;
TaskHandle_t Task3Handle = NULL;
#define configUSE_IDLE_HOOK 0


// Task 함수 프로토타입 선언
void SleepTask(void *pvParameters);
void RingTask(void *pvParameters);
void PutOffTask(void *pvParameters);

void SleepTask(void *pvParameters) {
    const char *message = (const char *) pvParameters;

    while (1) {
        printf("%s\n", message);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1초 주기
    }
}

void RingTask(void *pvParameters) {
    const char *message = (const char *) pvParameters;

    while (1) {
        printf("%s\n", message);
        vTaskResume(Task3Handle); // Task 3 resume
        vTaskDelay(pdMS_TO_TICKS(5000)); // 5초 주기
    }
}

void PutOffTask(void *pvParameters) {
    const char *message = (const char *) pvParameters;

    while (1) {
        printf("%s\n", message);
        vTaskSuspend(NULL); // 자기 자신을 suspend
    }
}

void vApplicationIdleHook(void) {
    // Idle Task가 실행될 때 수행할 작업이 없으면, 이 함수는 비워둘 수 있습니다.
    // 예: 저전력 모드로 진입, 상태 모니터링 등
}

int main(void) {
    // Task 1 생성
    xTaskCreate(SleepTask, "Sleep Task", configMINIMAL_STACK_SIZE, "Task 1: Sleeping...", 1, &Task1Handle);

    // Task 2 생성
    xTaskCreate(RingTask, "Ring Task", configMINIMAL_STACK_SIZE, "Task 2: Ringing...", 3, &Task2Handle);

    // Task 3 생성 (suspended 상태로 시작)
    xTaskCreate(PutOffTask, "Put off Task", configMINIMAL_STACK_SIZE, "Task 3: Put off", 2, &Task3Handle);
    vTaskSuspend(Task3Handle); // Task 3을 초기에는 suspend 상태로 유지

    // 스케줄러 시작
    vTaskStartScheduler();

    // 프로그램이 여기까지 도달하지 않도록 설정 (오류 발생 시)
    for(;;);
}
