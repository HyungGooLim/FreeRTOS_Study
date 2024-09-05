#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

// LED 상태 정의
#define LED_ON  1
#define LED_OFF 0

// 큐 핸들 및 세마포어 핸들 정의
QueueHandle_t Queue1;
QueueHandle_t Queue2;
SemaphoreHandle_t Semaphore;
TimerHandle_t TimerHandle;
SemaphoreHandle_t PrintSemaphore;  // 출력 순서를 제어하기 위한 세마포어

// LED 제어 함수 (예: GPIO 핀 제어)
void ControlLED(int ledNumber, int state) {
    printf("LED %d is now %s\n", ledNumber, state == LED_ON ? "ON" : "OFF");
}

// LED 1 제어 태스크: 1000ms 주기로 LED 1을 켜고 상태를 큐1에 기록
static void vLED1Task(void *pvParameters) {
    int led1State = LED_OFF;
    while (1) {
        led1State = !led1State;  // LED 상태 토글
        ControlLED(1, led1State);
        printf("LED 1 제어 태스크: LED1을 %s니다.\n", led1State == LED_ON ? "켭" : "끕");
        xQueueSend(Queue1, &led1State, 0);  // LED 1 상태를 큐1에 전송
        
        // 출력 순서 제어
        xSemaphoreGive(PrintSemaphore);  // 다음 태스크로 넘어가기 위해 세마포어를 'give'
        vTaskDelay(pdMS_TO_TICKS(1000));  // 1000ms 대기
    }
}

// 타이머 콜백 함수: 3000ms 주기로 실행되어 세마포어를 'give'
void vTimerCallback(TimerHandle_t xTimer) {
    printf("콜백함수: 타이머에 의해 실행됨\n");
    xSemaphoreGive(Semaphore);
}

// 핸들러 태스크: 세마포어를 받아 LED 2를 켜고 상태를 큐2에 기록
static void vHandlerTask(void *pvParameters) {
    int led2State = LED_OFF;
    while (1) {
        if (xSemaphoreTake(Semaphore, portMAX_DELAY) == pdTRUE) {
            led2State = LED_ON;
            ControlLED(2, led2State);
            xQueueSend(Queue2, &led2State, 0);  // LED 2 상태를 큐2에 전송
            printf("핸들러 태스크: LED2를 켭니다.\n");
        } else {
            printf("세마포어를 가져오지 못했습니다. LED2를 켜지 못했습니다.\n");
        }

        // 출력 순서 제어
        xSemaphoreGive(PrintSemaphore);  // 다음 태스크로 넘어가기 위해 세마포어를 'give'
    }
}

// LED 상태 태스크: 주기적으로 LED 상태를 출력 및 큐에 기록된 데이터 수 출력
static void LEDStatus(void *pvParameters) {
    int led1State, led2State;
    while (1) {
        xSemaphoreTake(PrintSemaphore, portMAX_DELAY);  // 출력 순서 제어를 위해 대기
        printf("LED State 태스크 시작\n");

        if (xQueueReceive(Queue1, &led1State, 0) == pdTRUE) {
            printf("LED 1 State: %s\n", led1State == LED_ON ? "ON" : "OFF");
        }
        if (xQueueReceive(Queue2, &led2State, 0) == pdTRUE) {
            printf("LED 2 State: %s\n", led2State == LED_ON ? "ON" : "OFF");
        }
        
        // 큐에 기록된 데이터 수 출력
        UBaseType_t queue1Count = uxQueueMessagesWaiting(Queue1);
        UBaseType_t queue2Count = uxQueueMessagesWaiting(Queue2);
        printf("Queue1에 기록된 데이터 수: %u\n", (unsigned int)queue1Count);
        printf("Queue2에 기록된 데이터 수: %u\n", (unsigned int)queue2Count);
        
        xSemaphoreGive(PrintSemaphore);  // 다음 태스크로 넘어가기 위해 세마포어를 'give'
        vTaskDelay(pdMS_TO_TICKS(1000));  // 1000ms 대기
    }
}

// 메인 함수: 태스크, 큐, 세마포어, 타이머를 생성하고 스케줄러 시작
int main(void) {
    // 큐 생성
    Queue1 = xQueueCreate(10, sizeof(int));
    Queue2 = xQueueCreate(10, sizeof(int));

    // 세마포어 생성
    Semaphore = xSemaphoreCreateBinary();
    PrintSemaphore = xSemaphoreCreateBinary();  // 출력 순서를 제어하기 위한 세마포어 생성

    // 타이머 생성
    TimerHandle = xTimerCreate("Timer", pdMS_TO_TICKS(3000), pdTRUE, NULL, vTimerCallback);

    // 타이머 시작
    xTimerStart(TimerHandle, 0);

    // 태스크 생성
    xTaskCreate(vLED1Task, "LED1Task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vHandlerTask, "HandlerTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(LEDStatus, "LEDStatusTask", configMINIMAL_STACK_SIZE, NULL, 2, NULL);  // 높은 우선순위로 설정

    // 스케줄러 시작
    vTaskStartScheduler();

    // 스케줄러가 실행 중이면 이 코드는 절대 실행되지 않음
    for (;;);
    return 0;
}
