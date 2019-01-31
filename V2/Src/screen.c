
/*
 * screen.c
 *
 *  Created on: Sep 16, 2017
 *      Author: alxhoff
*/

#include "error.h"
#include "screen_API.h"
#include "config.h"
#include "cmsis_os.h"

#include <stdlib.h>
#include <string.h>

typedef struct screen_device{
	int					rows;
	int 				cols;

	SemaphoreHandle_t	framebuffer_lock;
	char				**framebuffer;

	SemaphoreHandle_t 	cursor_lock;
	unsigned char 		cursor_on;
	int 				cursor_period;
	int 				cursor_location;
	TimerHandle_t		cursor_timer;

	osThreadId 			refresh_task;

	void				(*draw_text)(char **);
	void				(*draw_cursor)(unsigned char);
	void				(*clear_screen)(void);
	unsigned char		(*update_screen)(void);
	void				(*mv_cursor_left)(void);
	void				(*mv_cursor_right)(void);
} screen_device_t;

void screen_update(void);

screen_device_t screen_dev = {
		.cursor_period = SCREEN_CURSOR_PERIOD,
		.draw_text = &SCREEN_DRAW_TEXT,
		.draw_cursor = &SCREEN_DRAW_CURS,
		.clear_screen = &SCREEN_CLEAR,
		.update_screen = &SCREEN_REFRESH,
		.mv_cursor_left = &SCREEN_MV_CUR_LEFT,
		.mv_cursor_right = &SCREEN_MV_CUR_RIGHT
};

void screen_cursor_callback(TimerHandle_t timer) {
	xSemaphoreTake(screen_dev.cursor_lock, portMAX_DELAY);
	screen_dev.cursor_on = !screen_dev.cursor_on;
	xSemaphoreGive(screen_dev.cursor_lock);
}

void screen_set_framebuffer(char **fb) {
	screen_dev.framebuffer = fb;
}

void screen_refresh(void const *args){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	TickType_t xPeriod = 20;

	while(1){
		xSemaphoreTake(screen_dev.framebuffer_lock, portMAX_DELAY);
		xSemaphoreTake(screen_dev.cursor_lock, portMAX_DELAY);
		screen_dev.clear_screen();
		screen_dev.draw_text(screen_dev.framebuffer);
		screen_dev.draw_cursor(screen_dev.cursor_on);
		screen_dev.update_screen();
		xSemaphoreGive(screen_dev.cursor_lock);
		xSemaphoreGive(screen_dev.framebuffer_lock);

		xPeriod = SCREEN_PERIOD - (xLastWakeTime - xTaskGetTickCount());
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
	}
}

unsigned char screen_init(void) {
	screen_dev.rows = SCREEN_GET_ROWS;
	screen_dev.cols = SCREEN_GET_COLS;

	screen_dev.framebuffer = calloc(screen_dev.rows * screen_dev.cols,
				sizeof(char));
	if(!screen_dev.framebuffer)
		goto buffer_error;

	screen_dev.cursor_timer = xTimerCreate("Cursor Timer", screen_dev.cursor_period,
			1, NULL, screen_cursor_callback);
	if(!screen_dev.cursor_timer)
		goto timer_error;

	screen_dev.cursor_lock = xSemaphoreCreateMutex();
	if(!screen_dev.cursor_lock)
		goto c_lock_error;

	screen_dev.framebuffer_lock = xSemaphoreCreateMutex();
	if(!screen_dev.framebuffer_lock)
		goto f_lock_error;

	xTimerStart(screen_dev.cursor_timer, 0);

	osThreadDef(screenTask, screen_refresh, osPriorityNormal, 0, 128);
	screen_dev.refresh_task = osThreadCreate(osThread(screenTask), NULL);

	return 0;

	f_lock_error:
		vSemaphoreDelete(screen_dev.cursor_lock);
	c_lock_error:
		xTimerDelete(screen_dev.cursor_timer, portMAX_DELAY);
	timer_error:
		free(screen_dev.framebuffer);
	buffer_error:
		return -ENOINIT;
}


void screen_add_line(char *line) {
	if(screen_dev.framebuffer[screen_dev.rows - 1]) /* If the screen is full prepare for shift up */
		free(screen_dev.framebuffer[screen_dev.rows - 1]);

	for(unsigned char i = screen_dev.rows - 1; i > 0; i--)
		screen_dev.framebuffer[i] = screen_dev.framebuffer[i -1];

	screen_dev.framebuffer[0] = malloc(sizeof(char) * (strlen(line) + 1));
	strcpy(screen_dev.framebuffer[0], line);
}


