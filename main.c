#include "project.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "lvgl/lvgl.h"
#include "ili9341.h"

#include "spi_dma.h"
#include "disp_spi.h"

#include "driver_generic.h"
#include "psoc_platform.h"

#define NUM_ELEMENTS(x) (sizeof x / sizeof *x)

#define DISPLAY_BUFFER_SIZE (LV_HOR_RES_MAX * 10)

enum { FEED_FORM_CMD = 0x0C };

/* DMA Done handler */
CY_ISR_PROTO(spi_tx_dma_done);
/* SPI Tx Done handler */
CY_ISR_PROTO(spi_tx_isr_handler);

void timer_tick_handler(void);

/* LVGL function declaration */
static void create_application_screen(lv_obj_t *parent);
static int16_t get_encoder_diff(void);
static bool encoder_read(lv_indev_drv_t *drv, lv_indev_data_t *data);

/* MAIN APP */
lv_obj_t *selected_option;

LV_EVENT_CB_DECLARE(button_callback);
LV_EVENT_CB_DECLARE(parameter_list_container_event_handler);

static lv_obj_t *parameter_list_container;
static lv_obj_t *parameter_options_container;

enum {
    PARAMETER_LIST_BURST_TYPE   = 0,
    PARAMETER_LIST_BURST_PULSES,
    PARAMETER_LIST_PULSE_VOLTAGE,
    PARAMETER_LIST_COUNT,
};

static lv_obj_t *parameter_list[PARAMETER_LIST_COUNT];

lv_obj_t *shoot_btn;
lv_obj_t *shoot_btn_label;

/* Widgets on Type option */
lv_obj_t *continuous_cb;
lv_obj_t *custom_shoot_cb;
lv_obj_t *one_shoot_cb;
lv_obj_t *custom_spinbox;
lv_obj_t *type_param_ok_btn;

lv_obj_t *burst_type_list;

lv_obj_t *voltage_ddlist;
lv_obj_t *voltage_ok_btn;
lv_obj_t *pulses_ok_btn;

LV_EVENT_CB_DECLARE(burst_type_list_handler);
LV_EVENT_CB_DECLARE(type_param_ok_callback);

lv_obj_t *pulses_spinbox;

static void create_type_container_widgets(lv_obj_t *parent);
static void create_voltage_container_widgets(lv_obj_t *parent);
static void create_pulses_container_widgets(lv_obj_t *parent);

struct parameter_list_s {
    lv_obj_t *container;
    lv_obj_t *parameter[PARAMETER_LIST_COUNT];
};

struct parameter_list_s parameter_list_ssss;

lv_obj_t *one_shot_checkbox = NULL;
lv_obj_t *continous_checkbox = NULL;

lv_obj_t *v24_checkbox = NULL;
lv_obj_t *v48_checkbox = NULL;
lv_obj_t *v72_checkbox = NULL;
/* END OF MAIN APP */
uint8_t qd_current_cnt = 0;
uint8_t qd_old_cnt = 0;
static const uint8_t QUAD_DEC_SW_PRESSED = 0;

/* Groups */
static lv_group_t *parameter_list_g;
static lv_group_t *burst_type_params_g;
static lv_group_t *voltage_params_g;
static lv_group_t *pulses_params_g;

/* Input device */
static lv_indev_t* encoder_indev;

lv_obj_t *app_screen;

/* This struct is passed as lv_disp_drv_t.user_data, you should "register"
 * your platform specific functions in here. */
driver_generic_t ili9341_driver = {
    .display_backlight_control  =   psoc_display_backlight_control,
    .data_mode                  =   psoc_data_mode,
    .delay_ms                   =   psoc_delay_ms,
    .hw_reset                   =   NULL
};

int main(void)
{
    /* Register interrupt handlers */
    isr_SPI_TX_DMA_StartEx(spi_tx_dma_done);
    isr_SPI_TX_StartEx(spi_tx_isr_handler);
    
    /* Start peripherals */
    QuadDec_Start();
    UART_Start();
    SPI_Start();
    
    /* Initialize DMA Channel and configure it */
    spi_initialize_dma();
    
    /* Setup SysTick timer for 1ms periodic interrupt */
    CySysTickStart();
    CySysTickSetCallback(0, timer_tick_handler);
    CySysTickStop();
    CySysTickClear();
    
    /* Enable interrupts */
    CyGlobalIntEnable;
    
    /* Initialize LVGL */
    lv_init();
    
    /* LVGL color buffers */
    static lv_color_t buf1[DISPLAY_BUFFER_SIZE];
    static lv_color_t buf2[DISPLAY_BUFFER_SIZE];
    
    /* Configure working buffer */
    static lv_disp_buf_t disp_buf;
    lv_disp_buf_init(&disp_buf, buf1, buf2, DISPLAY_BUFFER_SIZE);
    
    /* Setup display driver */
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = ili9341_flush;
    disp_drv.buffer = &disp_buf;
    
    disp_drv.user_data = &ili9341_driver;
    
    lv_disp_drv_register(&disp_drv);
    
    /* Setup indev driver */
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    encoder_indev = lv_indev_drv_register(&indev_drv);
    
    /* Groups and input devices */
    pulses_params_g = lv_group_create();
    voltage_params_g = lv_group_create();
    parameter_list_g = lv_group_create();
    burst_type_params_g = lv_group_create();
    
    lv_indev_set_group(encoder_indev, parameter_list_g);
    
    /* Initialize ILI9341 display */
    ili9341_init(&disp_drv, NULL);
    
    /* Start SysTick */
    CySysTickStart();

    /* LVGL application */

    /* Main screen */
    app_screen = lv_obj_create(NULL, NULL);
    create_application_screen(app_screen);
    lv_scr_load(app_screen);

#if 0
    /* First screen */
    lv_obj_t *scr = lv_disp_get_scr_act(NULL);
    calibration_title_lbl = lv_label_create(scr, NULL);
    lv_label_set_text(calibration_title_lbl, "Touch screen calibration");
    lv_obj_align(calibration_title_lbl, NULL, LV_ALIGN_CENTER, 0, -50);
    
    calibration_timeout_lbl = lv_label_create(scr, NULL);
    lv_label_set_text_fmt(calibration_timeout_lbl, calibration_timeout_text_fmt, touch_calibration_time_s);
    lv_obj_align(calibration_timeout_lbl, NULL, LV_ALIGN_CENTER, 0, -20);
    
    /* TODO: Add callback to load main application (screen) */
    skip_calibration_btn = lv_btn_create(scr, NULL);
    lv_obj_align(skip_calibration_btn, NULL, LV_ALIGN_CENTER, 0, 50);
    lv_obj_set_event_cb(skip_calibration_btn, skip_calibration_cb);
    
    lv_group_add_obj(g, skip_calibration_btn);
    lv_group_focus_obj(skip_calibration_btn);
    
    skip_calibration_lbl = lv_label_create(skip_calibration_btn, NULL);
    lv_label_set_text(skip_calibration_lbl, "Skip calibration");
    
    /* Create task */
    timeout_task = lv_task_create(touch_calibration_timeout_task,
        TOUCH_CALIBRATION_TASK_PERIOD_MS, LV_TASK_PRIO_HIGH, NULL);
    lv_task_set_repeat_count(timeout_task, 6); /* Todo: replace magic number */
#endif

    while (1) {
        CyDelay(1);
        lv_task_handler();
    }
}

void timer_tick_handler(void)
{
    lv_tick_inc(1);
}

/* NOTE: The DMA done interrupt is being triggered when the DMA is done
 * transferring data into the SPI TX FIFO, this means it's gets triggered
 * even when the SPI TX FIFO still holds the last 4 byte of data.
 *
 * To know when spi is actually done transfering the data we can enable
 * the SPI_INT_ON_SPI_DONE (and SPI_INT_ON_TX_NOT_FULL). */
void spi_tx_dma_done(void)
{    
    spi_xfer_set_status(SPI_XFER_DONE);
    
    SPI_SetTxInterruptMode((0u << SPI_STS_TX_FIFO_NOT_FULL_SHIFT) | (1u << SPI_STS_SPI_DONE_SHIFT));
}

/* SPI interrupt handler */
void spi_tx_isr_handler(void)
{
    /* Clear the interrupt source flag */
    volatile uint8_t spi_tx_sts = SPI_ReadTxStatus();
    
    /* If the DMA is done and we get the SPI_STS_SPI_DONE interrupt then
     * the SPI FIFO is empty. */
    if ((SPI_XFER_DONE == spi_xfer_get_status()) && (SPI_STS_SPI_DONE & spi_tx_sts)) {
        
        SS_Write(1);
        
        /* Data transfer is done, we can disable SPI interrupts */
        SPI_SetTxInterruptMode(0);
        spi_xfer_on_completion();
        
        /* Let LVGL the flush has been finished */
        display_colors_sent_notification();
    }
}

static int16_t get_encoder_diff(void)
{
    int16_t diff = 0;
    
    qd_current_cnt = QuadDec_GetCounter();
    diff = qd_current_cnt - qd_old_cnt;
    qd_old_cnt = qd_current_cnt;

    return diff;
}

static bool encoder_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    (void) drv;
    
    /* Number of steps since the previous read */
    data->enc_diff = get_encoder_diff();
    
    /* Handle quadrature encoder button */
    if(QUAD_DEC_SW_PRESSED == QuadDec_SW_Read()){
        data->state = LV_INDEV_STATE_PR;
    } else{
        data->state = LV_INDEV_STATE_REL;
    }

    return false;
}

static void create_application_screen(lv_obj_t *parent)
{
#if 0
    lv_obj_t *tabview = lv_tabview_create(app_screen, NULL);

    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Configuration");
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "About");
    
    /* Calculate containers size */
    lv_tabview_ext_t *tabview_ext = lv_obj_get_ext_attr(tabview);
    
    lv_coord_t tb_content_height = lv_obj_get_height_fit(tabview_ext->content);
#endif

    lv_coord_t tb_content_height = lv_disp_get_ver_res(NULL);
    lv_coord_t container_height = tb_content_height - 50;

    lv_coord_t tb_content_width = lv_disp_get_hor_res(NULL);
    lv_coord_t container_width = (tb_content_width / 2) - 20;

    /* Add containers to tab1 */
    parameter_list_container = lv_cont_create(parent, NULL);
    lv_obj_set_size(parameter_list_container, container_width + 15, container_height);
    lv_obj_align(parameter_list_container, NULL, LV_ALIGN_IN_LEFT_MID, 15, 0);
    lv_cont_set_layout(parameter_list_container, LV_LAYOUT_COLUMN_LEFT);
    
    lv_obj_t *parameter_list_container_lbl = lv_label_create(parent, NULL);
    lv_label_set_text(parameter_list_container_lbl, "Parameters");
    lv_obj_align(parameter_list_container_lbl, parameter_list_container, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

    parameter_options_container = lv_cont_create(parent, NULL);
    lv_obj_set_size(parameter_options_container, container_width - 15, container_height);
    lv_obj_align(parameter_options_container, NULL, LV_ALIGN_IN_RIGHT_MID, -15, 0);
    /* TODO: Update layout */
    lv_cont_set_layout(parameter_options_container, LV_LAYOUT_COLUMN_MID);
    
    lv_obj_t *parameter_options_container_lbl = lv_label_create(parent, NULL);
    lv_label_set_text(parameter_options_container_lbl, "Options");
    lv_obj_align(parameter_options_container_lbl, parameter_options_container, LV_ALIGN_OUT_TOP_LEFT, 0, 0);
    
    /* Add widgets to parameter_list_container */

    /* Container title */
    parameter_list[PARAMETER_LIST_BURST_TYPE] = lv_checkbox_create(parameter_list_container, NULL);
    lv_checkbox_set_text(parameter_list[PARAMETER_LIST_BURST_TYPE], "Burst Type.");
    lv_obj_set_event_cb(parameter_list[PARAMETER_LIST_BURST_TYPE], parameter_list_container_event_handler);

    parameter_list[PARAMETER_LIST_BURST_PULSES] = lv_checkbox_create(parameter_list_container, NULL);
    lv_checkbox_set_text(parameter_list[PARAMETER_LIST_BURST_PULSES], "Pulses in burst.");
    lv_obj_set_event_cb(parameter_list[PARAMETER_LIST_BURST_PULSES], parameter_list_container_event_handler);

    parameter_list[PARAMETER_LIST_PULSE_VOLTAGE] = lv_checkbox_create(parameter_list_container, NULL);
    lv_checkbox_set_text(parameter_list[PARAMETER_LIST_PULSE_VOLTAGE], "Pulse Voltage.");
    lv_obj_set_event_cb(parameter_list[PARAMETER_LIST_PULSE_VOLTAGE], parameter_list_container_event_handler);
    
    shoot_btn = lv_btn_create(parameter_list_container, NULL);
    lv_btn_set_fit2(shoot_btn, LV_FIT_PARENT, LV_FIT_NONE);
    lv_obj_set_style_local_value_str(shoot_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "Shoot!");
    
    lv_obj_set_event_cb(shoot_btn, button_callback);

    /* Add objects to the group */
    lv_group_add_obj(parameter_list_g, parameter_list[PARAMETER_LIST_BURST_TYPE]);
    lv_group_add_obj(parameter_list_g, parameter_list[PARAMETER_LIST_BURST_PULSES]);
    lv_group_add_obj(parameter_list_g, parameter_list[PARAMETER_LIST_PULSE_VOLTAGE]);
    lv_group_add_obj(parameter_list_g, shoot_btn);
    
    /* Focus on the top checkbox */
    lv_group_focus_obj(parameter_list[PARAMETER_LIST_BURST_TYPE]);
    
    /* Create widgets for burst type option */
    create_type_container_widgets(parameter_options_container);
    /* Create widgets for voltage option */
    create_voltage_container_widgets(parameter_options_container);
    
    create_pulses_container_widgets(parameter_options_container);
}

/* NOTE: When any of the widgets on the list container is clicked the parameter container
 * needs to be cleared and load the necessary widgets.
 * 
 * TODO: Use lv_obj_clean(parameter_options_container) to clean the container. */
static void parameter_list_container_event_handler(lv_obj_t * obj, lv_event_t e)
{
    UART_PutString("parameter list handler");
    UART_PutHexByte((uint8_t) e);
    UART_PutCRLF();
    
    if(e == LV_EVENT_VALUE_CHANGED) {
        if (parameter_list[PARAMETER_LIST_BURST_TYPE] == obj && lv_checkbox_is_checked(obj)) {
            lv_checkbox_set_checked(parameter_list[PARAMETER_LIST_BURST_PULSES], false);
            lv_checkbox_set_checked(parameter_list[PARAMETER_LIST_PULSE_VOLTAGE], false);
            
            lv_indev_set_group(encoder_indev, burst_type_params_g);
            
            /* Enable widgets on parameter container */
            lv_obj_set_hidden(burst_type_list, false);
            lv_obj_set_hidden(custom_spinbox, false);
            lv_obj_set_hidden(type_param_ok_btn, false);
            
            lv_group_focus_obj(burst_type_list);
            
        } else if (parameter_list[PARAMETER_LIST_PULSE_VOLTAGE] == obj && lv_checkbox_is_checked(obj)) {
            lv_checkbox_set_checked(parameter_list[PARAMETER_LIST_BURST_TYPE], false);
            lv_checkbox_set_checked(parameter_list[PARAMETER_LIST_BURST_PULSES], false);
            
            UART_PutString("Setting voltage");
            
            lv_indev_set_group(encoder_indev, voltage_params_g);
            
            /* Enable widgets on parameter container */
            lv_obj_set_hidden(voltage_ddlist, false);
            lv_obj_set_hidden(voltage_ok_btn, false);
            
            lv_group_focus_obj(voltage_ddlist);
            
        } else if (parameter_list[PARAMETER_LIST_BURST_PULSES] == obj && lv_checkbox_is_checked(obj)) {
            lv_checkbox_set_checked(parameter_list[PARAMETER_LIST_BURST_TYPE], false);
            lv_checkbox_set_checked(parameter_list[PARAMETER_LIST_PULSE_VOLTAGE], false);
            
            lv_indev_set_group(encoder_indev, pulses_params_g);
            
            lv_obj_set_hidden(pulses_spinbox, false);
            lv_obj_set_hidden(pulses_ok_btn, false);
            
            lv_group_focus_obj(pulses_spinbox);
        }
    }
}

LV_EVENT_CB_DECLARE(button_callback)
{
    LV_UNUSED(obj);
    
    if ((LV_EVENT_CLICKED == e) || (LV_EVENT_LONG_PRESSED_REPEAT == e)) {
        /* TODO: Get the selected option on the dropdown list */
        uint16_t selected = lv_dropdown_get_selected(burst_type_list);
        UART_PutHexInt(selected);
        
        /* Get the pulse burst parameters and load them into the hardware */
        int32_t spinbox_value = lv_spinbox_get_value(custom_spinbox);
        UART_PutHexInt((uint16_t) spinbox_value & 0xFFFF);
        
        /* Shoot! */
    }
}

LV_EVENT_CB_DECLARE(custom_shoot_cb_handler)
{
    LV_UNUSED(obj);
    
    if (e == LV_EVENT_LONG_PRESSED) {
        UART_PutString("Long press\r\n");
    }
    
    if (e == LV_EVENT_VALUE_CHANGED) {
        lv_obj_set_hidden(custom_spinbox, false);
        
        lv_group_add_obj(burst_type_params_g, custom_spinbox);
        
        /* Focus the spinbox and enter in edit mode */
        lv_group_focus_obj(custom_spinbox);
        /* BUG: Spinbox doesn't enter edit mode */
        lv_group_set_editing(burst_type_params_g, true);
    }    
}

LV_EVENT_CB_DECLARE(type_param_ok_callback)
{
    LV_UNUSED(obj);
    
    if (e == LV_EVENT_PRESSED) {
        /* Hide Burst type widgets and go to the parameter list */
        lv_obj_set_hidden(custom_spinbox, true);
        lv_obj_set_hidden(burst_type_list, true);
        lv_obj_set_hidden(type_param_ok_btn, true);
        
        lv_indev_set_group(encoder_indev, parameter_list_g);
        UART_PutString("setting option");
    }
}

LV_EVENT_CB_DECLARE(custom_spinbox_handler)
{
    LV_UNUSED(obj);
    
    /* TODO: Check if the Custom option on the ddlist is selected */
    if ((LV_EVENT_FOCUSED == e)) {
        UART_PutString("Spinbox focused");
        lv_textarea_set_cursor_hidden(custom_spinbox, false);
    } else if ((LV_EVENT_DEFOCUSED == e)) {
        UART_PutString("Spinbox defocused");
        lv_textarea_set_cursor_hidden(custom_spinbox, true);
    } else {
        
    }
}

static void create_type_container_widgets(lv_obj_t *parent)
{    
    /* Widgets creation */
    burst_type_list = lv_dropdown_create(parent, NULL);
    custom_spinbox = lv_spinbox_create(parent, NULL);
    type_param_ok_btn = lv_btn_create(parent, NULL);
    
    lv_dropdown_set_options(burst_type_list, "One-shoot\nInfinite\nCustom");
    
    lv_spinbox_set_range(custom_spinbox, 1, 128);
    lv_spinbox_set_digit_format(custom_spinbox, 3, 0);
    
    /* BUG: Can't align spinbox label to the right of the spinbox with this code */
    lv_textarea_set_cursor_hidden(custom_spinbox, true); // hide the cursor
    lv_textarea_set_text_align(custom_spinbox, LV_LABEL_ALIGN_CENTER);
    
    /* Lets disable the spinbox so we can't focus in it with the encoder */
    lv_obj_set_state(custom_spinbox, LV_STATE_DISABLED);
    
    /* Widgets style */
    
    /* Set the width reduced by the left and right padding. */
    lv_coord_t parent_w = lv_obj_get_width(parent);
    lv_obj_set_width_fit(burst_type_list, parent_w - 5);
    lv_obj_set_width_fit(custom_spinbox, parent_w - 20);
    
    lv_obj_set_style_local_value_str(type_param_ok_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "OK!");
    lv_btn_set_fit2(type_param_ok_btn, LV_FIT_PARENT, LV_FIT_NONE);
    
    /* TODO: How to align the button to the bottom? */
    lv_obj_align(type_param_ok_btn, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
    
    /* Object event callbacks */
    lv_obj_set_event_cb(burst_type_list, burst_type_list_handler);
    // lv_obj_set_event_cb(custom_spinbox, custom_spinbox_handler);
    lv_obj_set_event_cb(type_param_ok_btn, type_param_ok_callback);
    
    /* Adding new widgets intot the group so the encoder can choose them */
    lv_group_add_obj(burst_type_params_g, burst_type_list);
    lv_group_add_obj(burst_type_params_g, custom_spinbox);
    lv_group_add_obj(burst_type_params_g, type_param_ok_btn);
    
    lv_obj_set_hidden(custom_spinbox, true);
    lv_obj_set_hidden(burst_type_list, true);
    lv_obj_set_hidden(type_param_ok_btn, true);
}

LV_EVENT_CB_DECLARE(burst_type_list_handler)
{
    LV_UNUSED(obj);
    
    /* This event only gets triggered when the selected option is changed, it
     * doesn't get triggered when the spinbox is unhidden. */
    if(e == LV_EVENT_VALUE_CHANGED) {
        uint16_t selected = lv_dropdown_get_selected(burst_type_list);
        UART_PutHexInt(selected);
        
        /* TODO: Use an enum instead of the magic number. */
        if (0x02 == selected) {
            lv_obj_set_state(custom_spinbox, LV_STATE_DEFAULT);
            lv_textarea_set_cursor_hidden(custom_spinbox, false);
        
            /* Focus the spinbox and enter in edit mode */
            lv_group_focus_obj(custom_spinbox);
            lv_group_set_editing(burst_type_params_g, true);
        } else {
            lv_obj_set_state(custom_spinbox, LV_STATE_DISABLED);
            lv_textarea_set_cursor_hidden(custom_spinbox, true);
        }
    }
}

LV_EVENT_CB_DECLARE(voltage_ok_btn_handler)
{
    LV_UNUSED(obj);
    
    if (e == LV_EVENT_PRESSED) {
        lv_obj_set_hidden(voltage_ddlist, true);
        lv_obj_set_hidden(voltage_ok_btn, true);
    
        lv_indev_set_group(encoder_indev, parameter_list_g);
        // lv_group_focus_obj(shoot_btn);
    }
}

LV_EVENT_CB_DECLARE(voltage_ddlist_handler)
{
    LV_UNUSED(obj);
    
    if (LV_EVENT_VALUE_CHANGED == e) {
        
    }
}

static void create_voltage_container_widgets(lv_obj_t *parent)
{
    /* Widgets creation */
    voltage_ddlist = lv_dropdown_create(parent, NULL);
    voltage_ok_btn = lv_btn_create(parent, NULL);
    
    lv_dropdown_set_options(voltage_ddlist, "24V\n48V\n72V");
    
    /* Adding new widgets into the group so the encoder can choose them */
    lv_group_add_obj(voltage_params_g, voltage_ddlist);
    lv_group_add_obj(voltage_params_g, voltage_ok_btn);
    
    /* Widgets style */
    lv_obj_set_style_local_value_str(voltage_ok_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "OK!");
    lv_btn_set_fit2(voltage_ok_btn,
        LV_FIT_PARENT,  /* horizontal */
        LV_FIT_NONE); /* vertical */
    
    /* Widgets event callbacks */
    lv_obj_set_event_cb(voltage_ddlist, voltage_ddlist_handler);
    lv_obj_set_event_cb(voltage_ok_btn, voltage_ok_btn_handler);
    
    lv_obj_set_hidden(voltage_ddlist, true);
    lv_obj_set_hidden(voltage_ok_btn, true);
}

LV_EVENT_CB_DECLARE(pulses_ok_btn_handler)
{
    LV_UNUSED(obj);
    
    if (LV_EVENT_PRESSED == e) {
        /* Hide Burst type widgets and go to the parameter list */
        lv_obj_set_hidden(pulses_ok_btn, true);
        lv_obj_set_hidden(pulses_spinbox, true);
        
        lv_indev_set_group(encoder_indev, parameter_list_g);
        lv_group_focus_obj(parameter_list[PARAMETER_LIST_BURST_PULSES]);
        lv_checkbox_set_checked(parameter_list[PARAMETER_LIST_BURST_PULSES], true);
    }
}

static void create_pulses_container_widgets(lv_obj_t *parent)
{
    pulses_spinbox = lv_spinbox_create(parent, NULL);
    pulses_ok_btn = lv_btn_create(parent, NULL);
    
    /* Adding new widgets into the group so the encoder can choose them */
    lv_group_add_obj(pulses_params_g, pulses_spinbox);
    lv_group_add_obj(pulses_params_g, pulses_ok_btn);
    
    lv_textarea_set_cursor_hidden(pulses_spinbox, false /* true */);
    lv_textarea_set_text_align(pulses_spinbox, LV_LABEL_ALIGN_CENTER);
    
    lv_coord_t parent_w = lv_obj_get_width(parent);
    lv_obj_set_width_fit(pulses_spinbox, parent_w - 20);
    
    lv_obj_set_style_local_value_str(pulses_ok_btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, "OK!");
    lv_btn_set_fit2(pulses_ok_btn,
        LV_FIT_PARENT,  /* horizontal */
        LV_FIT_NONE); /* vertical */
    
    lv_obj_set_event_cb(pulses_ok_btn, pulses_ok_btn_handler);
    
    lv_spinbox_set_range(pulses_spinbox, 1, 128);
    lv_spinbox_set_digit_format(pulses_spinbox, 3, 0);

    lv_obj_set_hidden(pulses_spinbox, true);
    lv_obj_set_hidden(pulses_ok_btn, true);
}

/* [] END OF FILE */
