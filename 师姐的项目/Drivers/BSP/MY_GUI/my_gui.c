#include "./BSP/MY_GUI/my_gui.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/CAP/cap.h"
#include "lvgl.h"
#include "math.h"
#include "./SYSTEM/sys/sys.h"
#include "motor.h"
#include "spwm.h"
#include "PWM.h"

uint32_t frequent;//PWM���е�Ƶ��
uint16_t degree;//����Ƕ�


/* ��ȡ��ǰ���Ļ�Ŀ�� */
#define scr_act_width()  lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())
#define SIN_POINTS_NUM   (18)                                  /* ���Ҳ���������� */

/* �¼��Ļص����������� */
static void btnm_cap_fuc_cb(lv_event_t  * e);/*���ݰ�ť���ܻص�*/
static void btn_win_close_cb(lv_event_t  * e);/*�����رջص�*/
static void kb_save_cb(lv_event_t  * e);/*�����������ݱ���ص�*/
static void textarea_change_focus_cb(lv_event_t  * e);/*�ı����������л��۽��ص�*/
static void kb_close_cb(lv_event_t  * e);/*�رռ��̻ص�*/
static void btnm_pwm_spwm_chang_cb(lv_event_t  * e);/*PWM/SPWM�����л��ص�*/
static void btn_motor_change_state_cb(lv_event_t  * e);/*���״̬�л��ص�*/
static void btnm_cap_cb(lv_event_t  * e);/*�����鰴ť����ص�*/
static void btn_sure_cb(lv_event_t  * e);/*�ı�ȷ�ϻص�����*/
static void dropdown_change_cb(lv_event_t  * e);/*SPWM���Ҳ�Ƶ���л��ص�����*/
static uint32_t string_to_num(const char str[]);/*�����ı�ת��Ϊ����*/
static void create_sin_buf(uint16_t maxval, uint16_t samples);/*������������*/


/*�����鰴ť������ı�*/
static const char *cap_map[] = { "C1", "C2", "C3","C4","C5", "\n",
                                 "C6", "C7", "C8","C9","C10", "\n",
                                 "C11", "C12", "C13","C14","C15", "\n",
                                 "C16", "C17", "C18","C19","C20", "\n",""};
/*���ݿ��ư�ť������ı�*/
static const char *fuc_map[] = { "Hand","Auto","Reset",""};
/*PWM/SPWM��ť������ı�*/
static const char *pwm_map[] = { "PWM","\n","SPWM",""};

uint8_t cap_flags[20] = {0};
static lv_point_t sin_line_points[SIN_POINTS_NUM];              /* ���Ҳ��������� */

//���õĲ���
static lv_obj_t *tabview;/* ���������ѡ� */
static lv_obj_t *table1_cap;/* ����ѡ� */
static lv_obj_t *table1_pwm;/* PWMѡ� */
static lv_obj_t *table1_motor;/* ���ѡ� */
static lv_obj_t *btnm_cap;/* ������İ�ť���� */
static lv_obj_t *btnm_fuc;/* �����鹦�ܵİ�ť���� */
static lv_obj_t *win;/* �����Զ����Ƶĵ��� */
static lv_obj_t *textarea_cap_fre;/* �Զ����Ƶ�Ƶ���������� */
static lv_obj_t *label_symbol_right;/* �Զ����Ƶ�Ƶ��������ɺ�ı�ʶ�� */
static lv_obj_t *kb;/*����*/
static lv_obj_t *win_content;/* �������� */
static lv_obj_t *btnm_pwm;/* PWM/SPWM�İ�ť���� */
static lv_obj_t *textarea_pwm_fre;/* PWM��Ƶ���������� */
static lv_obj_t *textarea_pwm_deg;/* PWM�ĽǶ��������� */
static lv_obj_t *label_state;/* �������״̬�ı� */
static lv_obj_t *label_state_symbol;/* �������״̬��־���� */
static lv_obj_t *btn_moter_up;/* �����ת��ť */
static lv_obj_t *btn_moter_down;/* �����ת��ť */
static lv_obj_t *btn_moter_stop;/* ���ֹͣ��ť */
static lv_obj_t *dropdown_sin;/*sinƵ��ѡ���������*/
static lv_obj_t *btn_sure;/*ȷ���ı���������ɵİ�ť*/
static lv_obj_t *btn_tips_tabl1;/*������ʾ�İ�ť*/
/* ��ʼ��ѡ� */
static void my_tabview_init(void)
{
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, scr_act_height()/6);        /* ����ѡ� */
    lv_obj_set_style_text_font(tabview, &lv_font_montserrat_14, LV_STATE_DEFAULT);

    table1_cap = lv_tabview_add_tab(tabview, "Cap");                                    /* ���ѡ�1 */
    table1_pwm = lv_tabview_add_tab(tabview, "Pwm");                                   /* ���ѡ�2 */
    table1_motor = lv_tabview_add_tab(tabview, "Motor");                               /* ���ѡ�3 */                               /* �������� */

    lv_obj_t *btn = lv_tabview_get_tab_btns(tabview);   /*��ȡѡ��İ�ť�����ñ���*/

    /* δѡ�еİ�ť */
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xb7472a), LV_PART_ITEMS|LV_STATE_DEFAULT);     /* ���ð�ť������ɫ����ɫ */
    lv_obj_set_style_bg_opa(btn, 200, LV_PART_ITEMS|LV_STATE_DEFAULT);                          /* ���ð�ť����͸���� */
    lv_obj_set_style_text_color(btn, lv_color_hex(0xf3f3f3), LV_PART_ITEMS|LV_STATE_DEFAULT);   /* ���ð�ť�ı���ɫ����ɫ */

    /* ѡ�еİ�ť */
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x22DED6), LV_PART_ITEMS|LV_STATE_CHECKED);     /* ���ð�ť������ɫ����ɫ */
    lv_obj_set_style_bg_opa(btn, 150, LV_PART_ITEMS|LV_STATE_CHECKED);                          /* ���ð�ť����͸���� */
    lv_obj_set_style_text_color(btn, lv_color_hex(0xb7472a), LV_PART_ITEMS|LV_STATE_CHECKED);   /* ���ð�ť�ı���ɫ����ɫ */
    lv_obj_set_style_border_width(btn, 0, LV_PART_ITEMS| LV_STATE_CHECKED);                     /* ���ð�ť�߿���Ϊ0 */

    /* 2������ */
    lv_obj_t *obj = lv_tabview_get_content(tabview);                                            /* ��ȡ���岿�� */
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x666666), LV_STATE_DEFAULT);                   /* ���ñ�����ɫ����ɫ */
    lv_obj_set_style_bg_opa(obj, 70, LV_STATE_DEFAULT);                                        /* ���ñ���͸���� */
	
}

/* ��ʼ��ѡ�1��CAP�еĲ��� */
static void my_table1_cap(void)
{
    /*�������ݿ��Ƶİ�ť����*/
    btnm_cap = lv_btnmatrix_create(table1_cap);                                         /* ������ť���� */
    lv_obj_set_size(btnm_cap, scr_act_width(), scr_act_width()* 1/2);                          /* ���ô�С */
    lv_btnmatrix_set_map(btnm_cap, cap_map);                                                        /* ���ð�ť */
    lv_obj_align_to(btnm_cap, table1_cap, LV_ALIGN_BOTTOM_MID, 0, 35);                             /* ����λ�� */
    lv_obj_set_style_text_font(btnm_cap, &lv_font_montserrat_14, LV_PART_ITEMS);
    lv_btnmatrix_set_btn_ctrl_all(btnm_cap, LV_BTNMATRIX_CTRL_DISABLED);    /* Ĭ���޷�ѡ�� */
    /*���ð�ť(���ݾ���)�������ɫ����С*/
    lv_obj_set_style_bg_color(btnm_cap,lv_color_hex(0x4D73E8), LV_PART_ITEMS);
    lv_obj_set_style_bg_color(btnm_cap,lv_color_hex(0xF02727), LV_STATE_CHECKED|LV_PART_ITEMS);
    lv_obj_set_style_border_width(btnm_cap, 0, LV_PART_MAIN);                                       /* ȥ������߿� */
    lv_obj_set_style_bg_opa(btnm_cap, 0, LV_PART_MAIN);                                             /* �������屳��͸���� */
    lv_obj_set_style_bg_opa(btnm_cap, 100, LV_PART_ITEMS);
    lv_obj_set_style_shadow_width(btnm_cap, 0, LV_PART_ITEMS);
    lv_obj_add_event_cb(btnm_cap, btnm_cap_cb, LV_EVENT_PRESSED, NULL);                                     /* ȥ����ť��Ӱ */
    /*���ð�ť����(���ܾ���)����ɫ����С*/
    btnm_fuc = lv_btnmatrix_create(table1_cap);
    lv_obj_set_size(btnm_fuc, 300, 80);
    lv_obj_align(btnm_fuc, LV_ALIGN_TOP_MID, 0, -20);
    lv_obj_set_style_border_width(btnm_fuc, 0, LV_PART_MAIN);                                       /* ȥ������߿� */
    lv_obj_set_style_border_width(btnm_fuc, 1, LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(btnm_fuc, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btnm_fuc, 0, LV_PART_ITEMS);
    lv_btnmatrix_set_map(btnm_fuc, fuc_map);
    lv_btnmatrix_set_btn_ctrl_all(btnm_fuc, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_obj_add_event_cb(btnm_fuc, btnm_cap_fuc_cb, LV_EVENT_PRESSED, NULL);

    btn_tips_tabl1 = lv_btn_create(table1_cap);
    lv_obj_set_size(btn_tips_tabl1, 20, 20);
    lv_obj_set_style_border_width(btn_tips_tabl1, 0, LV_STATE_DEFAULT);                                       /* ȥ������߿� */
    lv_obj_set_style_bg_opa(btn_tips_tabl1, 100, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btn_tips_tabl1, 0, LV_PART_ITEMS);
    lv_obj_align(btn_tips_tabl1, LV_ALIGN_TOP_LEFT, -12, 42);
    //lv_obj_add_event_cb(btn_tips_tabl1, );

    lv_obj_t * label = lv_label_create(btn_tips_tabl1);
    lv_obj_center(label);
    lv_label_set_text(label, LV_SYMBOL_EYE_OPEN);
}

/* ��ʼ��ѡ�2��PWM/SPWM�еĲ��� */
static void my_table1_pwm(void)
{
    /* PWM/SPWM�еİ�ť���� */
    btnm_pwm = lv_btnmatrix_create(table1_pwm);                                         /* ������ť���� */
    lv_obj_set_size(btnm_pwm, scr_act_width()*3/8, scr_act_height()*4/5);                          /* ���ô�С */
    lv_btnmatrix_set_map(btnm_pwm, pwm_map);                                                        /* ���ð�ť */
    lv_obj_align_to(btnm_pwm, table1_pwm, LV_ALIGN_LEFT_MID, -20, 0);                             /* ����λ�� */
    lv_obj_set_style_text_font(btnm_pwm, &lv_font_montserrat_14, LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(btnm_pwm, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(btnm_pwm, 1, LV_PART_ITEMS);
    lv_obj_set_style_border_width(btnm_pwm, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btnm_pwm, 0, LV_PART_ITEMS);
    lv_btnmatrix_set_btn_ctrl_all(btnm_pwm, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_obj_add_event_cb(btnm_pwm, btnm_pwm_spwm_chang_cb, LV_EVENT_PRESSED, NULL);

    /* SPWMǰ��Sin���� */
    lv_obj_t *sin_line = lv_line_create(table1_pwm);                                      /* �������� */
    create_sin_buf(scr_act_height()/11, SIN_POINTS_NUM);                                     /* �������Ҳ������ */
    lv_line_set_points(sin_line, sin_line_points, SIN_POINTS_NUM);                          /* ������������� */
    lv_obj_align(sin_line,LV_ALIGN_BOTTOM_MID, -16, -18 );                                                                /* ����λ�� */
    lv_obj_set_style_line_width(sin_line, 4, LV_PART_MAIN);                                 /* �����ߵĿ�� */
    lv_obj_set_style_line_color(sin_line, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN);  /* �����ߵ���ɫ */
    lv_obj_set_style_line_rounded(sin_line, true, LV_PART_MAIN);

    /* SPWM���Ҳ��������� */
    dropdown_sin = lv_dropdown_create(table1_pwm);
    lv_dropdown_set_options(dropdown_sin, "SIN_1HZ\nSIN_10Hz\nSIN_30Hz\nSIN_60Hz\nSIN_550Hz");
    lv_obj_set_size(dropdown_sin, 115, 38);
    lv_obj_align_to(dropdown_sin,sin_line,LV_ALIGN_OUT_RIGHT_MID, 11, 0);
    lv_obj_add_state(dropdown_sin, LV_STATE_DISABLED);
    lv_obj_set_style_border_width(dropdown_sin, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dropdown_sin, lv_color_hex(0x949494), LV_STATE_DISABLED);
    lv_obj_add_event_cb(dropdown_sin, dropdown_change_cb, LV_EVENT_VALUE_CHANGED, NULL);


    label_symbol_right = lv_label_create(table1_pwm);                                                     /* ������ǩ */
    lv_label_set_text(label_symbol_right, "");                                                              /* Ĭ�ϲ���ʾ */
    lv_obj_set_style_text_font(label_symbol_right, &lv_font_montserrat_14, LV_PART_MAIN);                                     /* �������� */
    lv_obj_align_to(label_symbol_right, dropdown_sin, LV_ALIGN_OUT_RIGHT_MID, 3, 0);
    /* PWMƵ������� */
    textarea_pwm_fre = lv_textarea_create(table1_pwm);
    lv_obj_set_size(textarea_pwm_fre, 90, 38);
    lv_obj_align(textarea_pwm_fre, LV_ALIGN_TOP_MID, 60, 5);
    lv_obj_add_state(textarea_pwm_fre, LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(textarea_pwm_fre, lv_color_hex(0x949494), LV_STATE_DISABLED);
    lv_obj_set_style_border_width(textarea_pwm_fre, 0, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(textarea_pwm_fre, textarea_change_focus_cb,LV_EVENT_ALL,NULL);

    /* PWMƵ�������ǩ */
    lv_obj_t * label_fre = lv_label_create(table1_pwm);
    lv_obj_set_style_text_font(label_fre, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_label_set_text(label_fre, "Fre/Hz:");
    lv_obj_align_to(label_fre,textarea_pwm_fre ,LV_ALIGN_OUT_LEFT_MID, -10, 0);

    /* PWM����Ƕ������ */
    textarea_pwm_deg = lv_textarea_create(table1_pwm);
    lv_obj_set_size(textarea_pwm_deg, 90, 38);
    lv_obj_add_state(textarea_pwm_deg, LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(textarea_pwm_deg, lv_color_hex(0x949494), LV_STATE_DISABLED);
    lv_obj_align_to(textarea_pwm_deg,textarea_pwm_fre, LV_ALIGN_OUT_BOTTOM_MID, 0, 3);
    lv_obj_set_style_border_width(textarea_pwm_deg, 0, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(textarea_pwm_deg, textarea_change_focus_cb,LV_EVENT_ALL,NULL);

    /* PWM����Ƕ������ǩ */
    lv_obj_t * label_deg = lv_label_create(table1_pwm);
    lv_obj_set_style_text_font(label_deg, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_label_set_text(label_deg, "Deg/��:");
    lv_obj_align_to(label_deg,label_fre ,LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    btn_sure = lv_btn_create(table1_pwm);
    lv_obj_set_size(btn_sure, 40, 70);
    lv_obj_set_style_bg_opa(btn_sure, 100, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btn_sure, 0, LV_PART_ITEMS);
    lv_obj_set_style_border_width(btn_sure, 1, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn_sure, lv_color_hex(0xff0000), LV_STATE_PRESSED);
    lv_obj_align_to(btn_sure, textarea_pwm_fre, LV_ALIGN_OUT_RIGHT_MID, 8, 20);
    lv_obj_add_state(btn_sure, LV_STATE_DISABLED);
    lv_obj_add_event_cb(btn_sure, btn_sure_cb, LV_EVENT_PRESSED, NULL);
    //lv_obj_add_flag(btn_sure, LV_OBJ_FLAG_CHECKABLE);

    lv_obj_t * label = lv_label_create(btn_sure);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_18, LV_PART_MAIN);
    lv_obj_center(label);
    lv_label_set_text(label, LV_SYMBOL_OK);
}

/* ��ʼ��ѡ�3��Motor�еĲ��� */
static void my_motor_tabl3(void)
{
    /* ״̬��ʶ�ı� */
    label_state = lv_label_create(table1_motor);
    lv_obj_set_style_text_font(label_state, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_label_set_text(label_state, "Motor state :  ");
    lv_obj_align(label_state, LV_ALIGN_TOP_MID, -30, 10);

    /* ״̬��ʶ���� */
    label_state_symbol = lv_label_create(table1_motor);
    lv_obj_set_style_text_font(label_state_symbol, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_label_set_text(label_state_symbol, LV_SYMBOL_PAUSE);
    lv_obj_align_to(label_state_symbol,label_state, LV_ALIGN_OUT_RIGHT_MID, 5, 2);
    lv_obj_set_style_text_color(label_state_symbol, lv_color_hex(0xFF2602), LV_STATE_DEFAULT);

    /* �������������� */
    lv_obj_t * label_motor_control = lv_obj_create(table1_motor);
    lv_obj_set_size(label_motor_control, 300, 120);
    lv_obj_align(label_motor_control, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_radius(label_motor_control, 10, LV_STATE_DEFAULT);

    lv_obj_t * label_motor_control_panel = lv_label_create(label_motor_control);
    lv_obj_set_style_text_font(label_motor_control_panel, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_label_set_text(label_motor_control_panel, "Motor control pannel");
    lv_obj_align(label_motor_control_panel, LV_ALIGN_TOP_MID, 0, 0);

    /*����������ť���Ƶ��������״̬*/
    btn_moter_up = lv_btn_create(label_motor_control);                                     /* �������ٰ�ť */
    lv_obj_set_size(btn_moter_up, scr_act_width() / 4, scr_act_height() / 5);       /* ���ð�ť��С */
    lv_obj_align(btn_moter_up, LV_ALIGN_BOTTOM_LEFT, -5, -5);           /* ���ð�ťλ�� */
    lv_obj_set_style_bg_color(btn_moter_up, lv_color_hex(0xff0000), LV_STATE_PRESSED);
    lv_obj_add_event_cb(btn_moter_up, btn_motor_change_state_cb, LV_EVENT_CLICKED, NULL);        /* ���ð�ť�¼� */

    lv_obj_t* label_up = lv_label_create(btn_moter_up);                                /* �������ٰ�ť��ǩ */
    lv_obj_set_style_text_font(label_up, &lv_font_montserrat_16, LV_PART_MAIN);                          /* �������� */
    lv_label_set_text(label_up, "Motor +");                                            /* ���ñ�ǩ�ı� */
    lv_obj_set_align(label_up,LV_ALIGN_CENTER);

    btn_moter_stop = lv_btn_create(label_motor_control);                                     /* �������ٰ�ť */
    lv_obj_set_size(btn_moter_stop, scr_act_width() / 4, scr_act_height() / 5);       /* ���ð�ť��С */
    lv_obj_align_to(btn_moter_stop, btn_moter_up,LV_ALIGN_OUT_RIGHT_MID, 18, 0);           /* ���ð�ťλ�� */
    lv_obj_set_style_bg_color(btn_moter_stop, lv_color_hex(0xef5f60), LV_STATE_DEFAULT);  /* ���ð�ť������ɫ��Ĭ�ϣ� */
    lv_obj_set_style_bg_color(btn_moter_stop, lv_color_hex(0xff0000), LV_STATE_PRESSED);
    lv_obj_add_event_cb(btn_moter_stop, btn_motor_change_state_cb, LV_EVENT_CLICKED, NULL);        /* ���ð�ť�¼� */

    lv_obj_t* label_stop = lv_label_create(btn_moter_stop);                                /* �������ٰ�ť��ǩ */
    lv_obj_set_style_text_font(label_stop, &lv_font_montserrat_16, LV_PART_MAIN);                          /* �������� */
    lv_label_set_text(label_stop, "Stop");                                            /* ���ñ�ǩ�ı� */
    lv_obj_set_align(label_stop,LV_ALIGN_CENTER);

    btn_moter_down = lv_btn_create(label_motor_control);                                     /* �������ٰ�ť */
    lv_obj_set_size(btn_moter_down, scr_act_width() / 4, scr_act_height() / 5);       /* ���ð�ť��С */
    lv_obj_align_to(btn_moter_down, btn_moter_stop,LV_ALIGN_OUT_RIGHT_MID, 17, 0);           /* ���ð�ťλ�� */
    lv_obj_set_style_bg_color(btn_moter_down, lv_color_hex(0xff0000), LV_STATE_PRESSED);
    lv_obj_add_event_cb(btn_moter_down, btn_motor_change_state_cb, LV_EVENT_CLICKED, NULL);        /* ���ð�ť�¼� */

    lv_obj_t* label_dowm = lv_label_create(btn_moter_down);                                /* �������ٰ�ť��ǩ */
    lv_obj_set_style_text_font(label_dowm, &lv_font_montserrat_16, LV_PART_MAIN);                          /* �������� */
    lv_label_set_text(label_dowm, "Motor -");                                            /* ���ñ�ǩ�ı� */
    lv_obj_set_align(label_dowm,LV_ALIGN_CENTER);
}

static void btnm_cap_fuc_cb(lv_event_t  * e)
{
    uint8_t id;
    lv_obj_t * target = lv_event_get_target(e);
    id = lv_btnmatrix_get_selected_btn(target);
    printf("%d ���� %s", id, lv_btnmatrix_get_btn_text(target, id));
    if (id == 0)
    {
        lv_btnmatrix_clear_btn_ctrl_all(btnm_cap, LV_BTNMATRIX_CTRL_DISABLED);//ʹ�ܰ�ť����
        lv_btnmatrix_set_btn_ctrl_all(btnm_cap, LV_BTNMATRIX_CTRL_CHECKABLE);
    }
    else if (id ==1)
    {
        lv_btnmatrix_clear_btn_ctrl(btnm_fuc,id, LV_BTNMATRIX_CTRL_CHECKED);
        lv_btnmatrix_clear_btn_ctrl_all(btnm_cap, LV_BTNMATRIX_CTRL_DISABLED);//ʹ�ܰ�ť����
        lv_btnmatrix_set_btn_ctrl_all(btnm_cap, LV_BTNMATRIX_CTRL_CHECKABLE);
        win = lv_win_create(lv_scr_act(),scr_act_height() / 12);
        lv_obj_set_size(win, scr_act_width() * 5 / 8,scr_act_height() * 4 / 7);
        //lv_obj_center(win);
        lv_obj_align_to(win, lv_scr_act(), LV_ALIGN_TOP_MID, 0 ,0);
        lv_obj_set_style_border_width(win, 1, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(win, lv_color_hex(0x8a8a8a), LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(win, 100, LV_STATE_DEFAULT);
        lv_obj_set_style_radius(win, 10, LV_STATE_DEFAULT);

        lv_obj_t * btn_setting = lv_win_add_btn(win, LV_SYMBOL_SETTINGS, 30);
        lv_obj_set_style_bg_opa(btn_setting, 0 ,LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(btn_setting, 0 ,LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(btn_setting, lv_color_hex(0x000000), LV_STATE_DEFAULT);

        lv_obj_t * title = lv_win_add_title(win, "Setting");
        lv_obj_set_style_text_font(title, &lv_font_montserrat_12, LV_STATE_DEFAULT);

        lv_obj_t * btn_close = lv_win_add_btn(win, LV_SYMBOL_CLOSE, 30);
        lv_obj_set_style_bg_opa(btn_close, 0 ,LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(btn_close, 0 ,LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(btn_close, lv_color_hex(0x000000), LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(btn_close, lv_color_hex(0xff0000), LV_STATE_PRESSED);
        lv_obj_add_event_cb(btn_close, btn_win_close_cb, LV_EVENT_CLICKED, NULL);

        win_content = lv_win_get_content(win);
        lv_obj_set_style_bg_color(win_content, lv_color_hex(0xffffff),LV_STATE_DEFAULT);

        lv_obj_t * label_fre = lv_label_create(win_content);
        lv_obj_set_style_text_font(label_fre, &lv_font_montserrat_14, LV_PART_MAIN);
        lv_label_set_text(label_fre, "Fre/Hz:");
        lv_obj_align(label_fre, LV_ALIGN_CENTER, -50, -10);


        kb = lv_keyboard_create(lv_scr_act());
        textarea_cap_fre = lv_textarea_create(win_content);
        lv_obj_set_size(textarea_cap_fre, 90, 38);
        lv_obj_align_to(textarea_cap_fre, label_fre, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
        lv_keyboard_set_textarea(kb, textarea_cap_fre);
        lv_keyboard_set_popovers(kb ,true);
        lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
        lv_obj_add_event_cb(kb, kb_save_cb, LV_EVENT_PRESSED, NULL);

    
    }
    else if (id == 2)
    {
        for (int i = 0; i < 20; i++)
        {
            cap_flags[i] = 0;
            cap_change(cap_flags[i], i+1);
        }
        lv_btnmatrix_clear_btn_ctrl_all(btnm_cap, LV_BTNMATRIX_CTRL_CHECKED);
        lv_btnmatrix_set_btn_ctrl(btnm_fuc,id, LV_BTNMATRIX_CTRL_CHECKED);
        lv_btnmatrix_set_btn_ctrl_all(btnm_cap, LV_BTNMATRIX_CTRL_DISABLED);
        lv_btnmatrix_clear_btn_ctrl(btnm_fuc, 0,LV_BTNMATRIX_CTRL_CHECKED);
        lv_btnmatrix_clear_btn_ctrl(btnm_fuc, 1,LV_BTNMATRIX_CTRL_CHECKED);
    }
}
static void btn_win_close_cb(lv_event_t  * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_add_flag(win, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }

}
static void btnm_cap_cb(lv_event_t  * e)
{
    uint8_t id;
    lv_obj_t * target = lv_event_get_target(e);
    id = lv_btnmatrix_get_selected_btn(target);
    if (cap_flags[id] == 0)
        cap_flags[id] = 1;
    else
        cap_flags[id] = 0;
    cap_change(cap_flags[id], id+1);
}
static void kb_save_cb(lv_event_t  * e)
{
    //uint8_t id;
    lv_obj_t * target = lv_event_get_target(e);
    //id = lv_btnmatrix_get_selected_btn(target);
    //printf("%d ���� %s", id, lv_btnmatrix_get_btn_text(target, id));
    
}
static void btn_motor_change_state_cb(lv_event_t  * e)
{
    lv_obj_t *target = lv_event_get_target(e);      /* ��ȡ����Դ */

    if (target == btn_moter_up)
    {
        lv_label_set_text(label_state_symbol, LV_SYMBOL_PLUS);
        __HAL_TIM_SET_COMPARE(&Motor_tim4, TIM_CHANNEL_2, 500);
        HAL_TIM_PWM_Start(&Motor_tim4, TIM_CHANNEL_2);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3,GPIO_PIN_SET);
    }
    else if (target == btn_moter_down)
    {
        lv_label_set_text(label_state_symbol, LV_SYMBOL_MINUS);
        __HAL_TIM_SET_COMPARE(&Motor_tim4, TIM_CHANNEL_2, 500);
        HAL_TIM_PWM_Start(&Motor_tim4, TIM_CHANNEL_2);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3,GPIO_PIN_RESET);
    }
    else if (target == btn_moter_stop)
    {
        lv_label_set_text(label_state_symbol, LV_SYMBOL_PAUSE);
        __HAL_TIM_SET_COMPARE(&Motor_tim4, TIM_CHANNEL_2, 0);
    }
}
static void dropdown_change_cb(lv_event_t  * e)
{
    uint8_t id;
    char buff[15];
    lv_obj_t *target = lv_event_get_target(e);      /* ��ȡ����Դ */
    id = lv_dropdown_get_selected(target);
    lv_dropdown_get_selected_str(target, buff, sizeof(buff));
    printf("id = %d, %s\r\n", id, buff);
    lv_label_set_text(label_symbol_right, LV_SYMBOL_OK);

    RCC->APB2RSTR |= (1<<0);
    RCC->APB2RSTR |= (1<<1);
    RCC->APB1RSTR |= (1<<0);

    RCC->APB2RSTR &= ~(1<<0);
    RCC->APB2RSTR &= ~(1<<1);
    RCC->APB1RSTR &= ~(1<<0);

    switch(id)
    {
        case 0:SPWM_init(SIN_1Hz);break;
        case 1:SPWM_init(SIN_10Hz);break;
        case 2:SPWM_init(SIN_30Hz);break;
        case 3:SPWM_init(SIN_60Hz);break;
        case 4:SPWM_init(SIN_550Hz);break;

        default:break;
    }
}
static uint32_t string_to_num(const char str[])
{
    uint8_t i = 0;
    uint32_t num = 0;

    for(i=0;i < strlen(str);i++)
    {
        num=num*10+str[i]-'0';
    }
    return num;
}
static void textarea_change_focus_cb(lv_event_t  * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if (code == LV_EVENT_FOCUSED)
    {
        lv_keyboard_set_textarea(kb, target);
    }
}
static void kb_close_cb(lv_event_t  * e)
{
    uint8_t id;
    lv_obj_t * target = lv_event_get_target(e);
    id = lv_btnmatrix_get_selected_btn(target);
    {
        if (id == 3)
        {
            lv_obj_del(kb);
            lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        }
    }
}
static void btn_sure_cb(lv_event_t  * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_PRESSED)
    {
        printf("%s\r\n",lv_textarea_get_text(textarea_pwm_fre));
        //printf("Translate to %u\r\n", string_to_num(lv_textarea_get_text(textarea_pwm_fre)));
        frequent = string_to_num(lv_textarea_get_text(textarea_pwm_fre));
        printf("Translate to %u\r\n", frequent);
        printf("%s\r\n",lv_textarea_get_text(textarea_pwm_deg));
        //printf("Translate to %u\r\n", string_to_num(lv_textarea_get_text(textarea_pwm_deg)));
        degree = string_to_num(lv_textarea_get_text(textarea_pwm_deg));
        printf("Translate to %u\r\n", degree);

        
        
        //PWM_tim8.Instance->SR &= ~(1<<6);
        //PWM_tim8.Instance->CR1 &= ~(1<<0);
        //��λ����
        RCC->APB2RSTR |= (1<<0);
        RCC->APB2RSTR |= (1<<1);
        RCC->APB1RSTR |= (1<<0);

        RCC->APB2RSTR &= ~(1<<0);
        RCC->APB2RSTR &= ~(1<<1);
        RCC->APB1RSTR &= ~(1<<0);
        PWM_PHASE_SHIFT(frequent, degree);
        
         
    }
}
static void btnm_pwm_spwm_chang_cb(lv_event_t  * e)
{
    uint8_t id;
    lv_obj_t * target = lv_event_get_target(e);
    id = lv_btnmatrix_get_selected_btn(target);
    if (id == 0)
    {
        kb = lv_keyboard_create(table1_pwm);
        lv_obj_set_size(kb, 320, 90);
        lv_obj_add_event_cb(kb, kb_close_cb, LV_EVENT_PRESSED, NULL);
        lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
        lv_btnmatrix_clear_btn_ctrl(target, 1, LV_BTNMATRIX_CTRL_CHECKED);
        lv_obj_clear_state(textarea_pwm_fre, LV_STATE_DISABLED);
        lv_obj_clear_state(textarea_pwm_deg, LV_STATE_DISABLED);
        lv_obj_clear_state(btn_sure, LV_STATE_DISABLED);
        lv_obj_add_state(dropdown_sin, LV_STATE_DISABLED);
        lv_btnmatrix_clear_btn_ctrl(target,id, LV_BTNMATRIX_CTRL_CHECKED);
    }
    else if (id == 1)
    {
        lv_btnmatrix_clear_btn_ctrl(target, 0, LV_BTNMATRIX_CTRL_CHECKED);
        lv_obj_add_state(textarea_pwm_fre, LV_STATE_DISABLED);
        lv_obj_add_state(textarea_pwm_deg, LV_STATE_DISABLED);
        lv_obj_add_state(btn_sure, LV_STATE_DISABLED);
        lv_obj_clear_state(dropdown_sin, LV_STATE_DISABLED);
    }
}
static void create_sin_buf(uint16_t maxval, uint16_t samples)
{
    uint16_t i;
    float y = 0;

    /*
     * ���Ҳ���С������Ϊ2�У�Լ����2 * 3.1415926
     * ���������ڵ���������x���ϵļ�� = 2 * 3.1415926 / ����������
     */
    float inc = (2 * 3.1415926) / samples;                      /* ���������������x���� */

    for (i = 0; i < samples; i++)                               /* ��������samples���� */
    {
        /*
         * ���Ҳ���������ʽ��y = Asin(wx + �գ�+ b
         * ����ÿ�����yֵ������ֵ�Ŵ�maxval����������������ƫ��maxval����������
         */
        y = maxval * sin(inc * i) + maxval;

        sin_line_points[i].x = 2 * i;                           /* ����x������ */
        sin_line_points[i].y = y;                               /* ����y������ */
    }
}


/*������Ի�*/
void my_gui(void)
{
    my_tabview_init();
    my_table1_cap();
    my_table1_pwm();
    my_motor_tabl3();
    
}

