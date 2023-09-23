/*
    11UILayer Max UI object
    by 11olsen.de
*/

#include "ext.h"                            
#include "ext_obex.h"                        
#include "jpatcher_api.h"
#include "jgraphics.h"
#include "ext_parameter.h"
#include "ext_dictionary.h"
#include "jpatcher_utils.h"

#define DEBUG 0

typedef struct _UILayerDef
{
    t_jbox u_box;                        
    void* u_out1;                        
    void* u_out2;
    void* u_out3;
    void* u_out4;
    void* u_out5;
    void* u_out6;
    //void* u_out7;
    //void* u_out8;

    t_object* jp;
    t_object* jb;
    char mode;
    t_atom targetatom[1];
    t_atom cursortatom[1];
    t_symbol* targetname;
    t_object* targetBox;
    t_pt prevmousemove;
    t_pt prevdragpos;
    //int targetdragmode;
    char targetmt;
    char we_are_loaded;
    char isinlive;

    //PASS
    char pass_mousedown;
    char pass_mousedrag;
    char pass_mouseup;
    char pass_mouseenter;
    char pass_mouseleave;
    char pass_mousemove;
    char pass_mousedoubleclick;
    char pass_mousewheel;
    char pass_focusgained;
    char pass_focuslost;
    //char pass_mousedragdelta;
    char pass_key;
    char pass_keyup;

    //OUT
    char out_mousedown;
    char out_mousedrag;
    char out_mouseup;
    char out_mouseenter;
    char out_mouseleave;
    char out_mousemove;
    char out_mousedoubleclick;
    char out_mousewheel;
    char out_focusgained;
    char out_focuslost;
    char out_mousedragdelta;
    char out_key;
    char out_keyup;

    // OPTIONS
    
    
    char ignorewheel;
    char force_shift_drag;
    char draghide;
    char dragdelta;
    char wheel_delta_clip;
    

} t_UILayerDef;



//##### Def
void* UILayerDef_new(t_symbol *s, long argc, t_atom *argv);
void UILayerDef_free(t_UILayerDef* x);
void UILayerDef_assist(t_UILayerDef* x, void *b, long m, long a, char *s);
void UILayerDef_bang(t_UILayerDef* x);

void UILayerDef_mousedown(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers);
void UILayerDef_mousedrag(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers);
void UILayerDef_mouseup(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers);
void UILayerDef_mouseenter(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers);
void UILayerDef_mouseleave(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers);
void UILayerDef_mousemove(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers);
void UILayerDef_mousewheel(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers, double x_inc, double y_inc);
void UILayerDef_mousedoubleclick(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers);
void UILayerDef_focusgained(t_UILayerDef* x, t_object* patcherview);
void UILayerDef_focuslost(t_UILayerDef* x, t_object* patcherview);
void UILayerDef_mousedragdelta(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers);
long UILayerDef_key(t_UILayerDef* x, t_object* patcherview, long keycode, long modifiers, long textcharacter);
long UILayerDef_keyup(t_UILayerDef* x, t_object* patcherview, long keycode, long modifiers, long textcharacter);

t_max_err UILayerDef_setattr_target(t_UILayerDef* x, void* attr, long ac, t_atom* av);
t_max_err UILayerDef_setattr_draghide(t_UILayerDef* x, void* attr, long ac, t_atom* av);
t_max_err UILayerDef_setattr_ignorewheel(t_UILayerDef* x, void* attr, long ac, t_atom* av);
void UILayerDef_do_ignorewheel(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);
void UILayerDef_do_draghide(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);

void UILayerDef_setvalue(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);
void UILayerDef_addtovalue(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);
void UILayerDef_send(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);
void UILayerDef_do_send(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);
void UILayerDef_setcursorpos(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);
void UILayerDef_cursor(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);
void UILayerDef_do_cursor(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);
void UILayerDef_test(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);
void UILayerDef_do_focus(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);

void UILayerDef_targetinfo(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);
void UILayerDef_putontop(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);

void UILayerDef_closemenu(t_UILayerDef* x);
void UILayerDef_loaded(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv);

t_max_err UILayerDef_notify(t_UILayerDef* x, t_symbol* s, t_symbol* msg, void* sender, void* data);

//void uisimp_paint(t_uisimp *x, t_object *patcherview);
//void uisimp_getdrawparams(t_uisimp *x, t_object *patcherview, t_jboxdrawparams *params);
//long uisimp_hittest(t_uisimp* x, t_object* patcherview, t_pt pt);


static t_class* s_UILayerDef_class;
static t_symbol* _sym_keyup;
static t_symbol* _sym_mt_mousedrag;
static t_symbol* _sym_mt_mousedragdelta;
static t_symbol* _sym_mt_mousedown;
static t_symbol* _sym_mt_mouseup;
static t_symbol* _sym_mt_mouseenter;
static t_symbol* _sym_mt_mouseleave;
static t_symbol* _sym_setcursorpos_patcher;
static t_symbol* _sym_setcursorpos_box;
static t_symbol* _sym_setcursorpos_screen;
static t_symbol* _sym_setcursorpos_targetbox;
static t_symbol* _sym_hidecursor;
static t_symbol* _sym_showcursor;
static t_symbol* _sym_cursor_patcher;
static t_symbol* _sym_cursor_all;
static t_symbol* _sym_cursor_objects;
static t_symbol* _sym_cursor_target;
static t_symbol* _sym_ignorewheel;
static t_symbol* _sym_isinlive;

void UILayerDef_test(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
{
    post("!!!!!!!!!!!!!!");
    
}

void ext_main(void *r)
{
    common_symbols_init();
    char* cat1 = "Pass";
    char* cat2 = "Output";
    char* cat3 = "Options";
    t_class* c;
    c = class_new("11UILayer", (method)UILayerDef_new, (method)UILayerDef_free, sizeof(t_UILayerDef), 0L, A_GIMME, 0);

    c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
    jbox_initclass(c, 0); //JBOX_FIXWIDTH | JBOX_COLOR
    
    class_addmethod(c, (method)UILayerDef_notify, "notify", A_CANT, 0);
    class_addmethod(c, (method)UILayerDef_bang, "bang", 0);
    class_addmethod(c, (method)UILayerDef_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)UILayerDef_mousedown, "mousedown", A_CANT, 0);
    class_addmethod(c, (method)UILayerDef_mousedrag, "mousedrag", A_CANT, 0);
    class_addmethod(c, (method)UILayerDef_mouseup, "mouseup", A_CANT, 0);
    class_addmethod(c, (method)UILayerDef_mouseenter, "mouseenter", A_CANT, 0);
    class_addmethod(c, (method)UILayerDef_mouseleave, "mouseleave", A_CANT, 0);
    class_addmethod(c, (method)UILayerDef_mousemove, "mousemove", A_CANT, 0);
    //class_addmethod(c, (method)UILayerDef_mousewheel, "mousewheel", A_CANT, 0); // adding this dynamically
    class_addmethod(c, (method)UILayerDef_mousedoubleclick, "mousedoubleclick", A_CANT, 0);
    class_addmethod(c, (method)UILayerDef_focusgained, "focusgained", A_CANT, 0);
    class_addmethod(c, (method)UILayerDef_focuslost, "focuslost", A_CANT, 0);
    class_addmethod(c, (method)UILayerDef_mousedragdelta, "mousedragdelta", A_CANT, 0);
    class_addmethod(c, (method)UILayerDef_key, "key", A_CANT, 0);
    class_addmethod(c, (method)UILayerDef_keyup, "keyup", A_CANT, 0);
    
    
    class_addmethod(c, (method)UILayerDef_addtovalue, "addtovalue", A_GIMME, 0);
    class_addmethod(c, (method)UILayerDef_setvalue, "setvalue", A_GIMME, 0);
    class_addmethod(c, (method)UILayerDef_send, "send", A_GIMME, 0);
    class_addmethod(c, (method)UILayerDef_cursor, "cursor", A_GIMME, 0);
    class_addmethod(c, (method)UILayerDef_cursor, "cursor_patcher", A_GIMME, 0);
    class_addmethod(c, (method)UILayerDef_cursor, "cursor_all", A_GIMME, 0);
    class_addmethod(c, (method)UILayerDef_cursor, "cursor_objects", A_GIMME, 0);
    class_addmethod(c, (method)UILayerDef_cursor, "cursor_target", A_GIMME, 0);
    class_addmethod(c, (method)UILayerDef_cursor, "hidecursor", A_GIMME, 0);
    class_addmethod(c, (method)UILayerDef_cursor, "showcursor", A_GIMME, 0);

    class_addmethod(c, (method)UILayerDef_setcursorpos, "setcursorpos_targetbox", A_GIMME, 0);
    class_addmethod(c, (method)UILayerDef_setcursorpos, "setcursorpos_box", A_GIMME, 0);
    class_addmethod(c, (method)UILayerDef_setcursorpos, "setcursorpos_patcher", A_GIMME, 0);
    class_addmethod(c, (method)UILayerDef_setcursorpos, "setcursorpos_screen", A_GIMME, 0);

    class_addmethod(c, (method)UILayerDef_closemenu, "closemenu", 0);

    class_addmethod(c, (method)UILayerDef_putontop, "putontop", A_GIMME, 0);
    class_addmethod(c, (method)UILayerDef_targetinfo, "targetinfo", A_GIMME, 0);
    //class_addmethod(c, (method)UILayerDef_test, "test", A_GIMME, 0);


    //ATTR
    CLASS_ATTR_CHAR(c, "mode", 0, t_UILayerDef, mode);
    CLASS_ATTR_LABEL(c, "mode", 0, "operation mode, set at object creation");
    CLASS_ATTR_SAVE(c, "mode", 0);

    CLASS_ATTR_ATOM(c, "target", 0, t_UILayerDef, targetatom);
    CLASS_ATTR_ACCESSORS(c, "target", (method)NULL, (method)UILayerDef_setattr_target);
    CLASS_ATTR_STYLE_LABEL(c, "target", 0, "text", "scripting name of target object");
    CLASS_ATTR_SAVE(c, "target", 0);



    //PASS
    CLASS_ATTR_CHAR(c, "pass_mouseup", 0, t_UILayerDef, pass_mouseup);
    CLASS_ATTR_STYLE_LABEL(c, "pass_mouseup", 0, "onoff", "to target object");
    CLASS_ATTR_SAVE(c, "pass_mouseup", 0);

    CLASS_ATTR_CHAR(c, "pass_mousedown", 0, t_UILayerDef, pass_mousedown);
    CLASS_ATTR_STYLE_LABEL(c, "pass_mousedown", 0, "onoff", "to target object");
    CLASS_ATTR_SAVE(c, "pass_mousedown", 0);

    CLASS_ATTR_CHAR(c, "pass_mousedrag", 0, t_UILayerDef, pass_mousedrag);
    CLASS_ATTR_STYLE_LABEL(c, "pass_mousedrag", 0, "onoff", "or mousedragdelta to target object");
    CLASS_ATTR_SAVE(c, "pass_mousedrag", 0);

    CLASS_ATTR_CHAR(c, "pass_mouseenter", 0, t_UILayerDef, pass_mouseenter);
    CLASS_ATTR_STYLE_LABEL(c, "pass_mouseenter", 0, "onoff", "to target object");
    CLASS_ATTR_SAVE(c, "pass_mouseenter", 0);

    CLASS_ATTR_CHAR(c, "pass_mouseleave", 0, t_UILayerDef, pass_mouseleave);
    CLASS_ATTR_STYLE_LABEL(c, "pass_mouseleave", 0, "onoff", "to target object");
    CLASS_ATTR_SAVE(c, "pass_mouseleave", 0);

    CLASS_ATTR_CHAR(c, "pass_mousemove", 0, t_UILayerDef, pass_mousemove);
    CLASS_ATTR_STYLE_LABEL(c, "pass_mousemove", 0, "onoff", "to target object");
    CLASS_ATTR_SAVE(c, "pass_mousemove", 0);

    CLASS_ATTR_CHAR(c, "pass_mousedoubleclick", 0, t_UILayerDef, pass_mousedoubleclick);
    CLASS_ATTR_STYLE_LABEL(c, "pass_mousedoubleclick", 0, "onoff", "to target object");
    CLASS_ATTR_SAVE(c, "pass_mousedoubleclick", 0);

    CLASS_ATTR_CHAR(c, "pass_mousewheel", 0, t_UILayerDef, pass_mousewheel);
    CLASS_ATTR_STYLE_LABEL(c, "pass_mousewheel", 0, "onoff", "to target object");
    CLASS_ATTR_SAVE(c, "pass_mousewheel", 0);

    CLASS_ATTR_CHAR(c, "pass_focusgained", 0, t_UILayerDef, pass_focusgained);
    CLASS_ATTR_STYLE_LABEL(c, "pass_focusgained", 0, "onoff", "to target object");
    CLASS_ATTR_SAVE(c, "pass_focusgained", 0);

    CLASS_ATTR_CHAR(c, "pass_focuslost", 0, t_UILayerDef, pass_focuslost);
    CLASS_ATTR_STYLE_LABEL(c, "pass_focuslost", 0, "onoff", "to target object");
    CLASS_ATTR_SAVE(c, "pass_focuslost", 0);

    /*CLASS_ATTR_CHAR(c, "pass_mousedragdelta", 0, t_UILayerDef, pass_mousedragdelta);
    CLASS_ATTR_STYLE_LABEL(c, "pass_mousedragdelta", 0, "onoff", "to target object");
    CLASS_ATTR_SAVE(c, "pass_mousedragdelta", 0);*/

    CLASS_ATTR_CHAR(c, "pass_key", 0, t_UILayerDef, pass_key);
    CLASS_ATTR_STYLE_LABEL(c, "pass_key", 0, "onoff", "to target object");
    CLASS_ATTR_SAVE(c, "pass_key", 0);

    CLASS_ATTR_CHAR(c, "pass_keyup", 0, t_UILayerDef, pass_keyup);
    CLASS_ATTR_STYLE_LABEL(c, "pass_keyup", 0, "onoff", "to target object");
    CLASS_ATTR_SAVE(c, "pass_keyup", 0);

    CLASS_ATTR_CATEGORY(c, "pass_mousedown", 0, cat1);
    CLASS_ATTR_CATEGORY(c, "pass_mousedrag", 0, cat1);
    CLASS_ATTR_CATEGORY(c, "pass_mouseup", 0, cat1);
    CLASS_ATTR_CATEGORY(c, "pass_mouseenter", 0, cat1);
    CLASS_ATTR_CATEGORY(c, "pass_mouseleave", 0, cat1);
    CLASS_ATTR_CATEGORY(c, "pass_mousemove", 0, cat1);
    CLASS_ATTR_CATEGORY(c, "pass_mousedoubleclick", 0, cat1);
    CLASS_ATTR_CATEGORY(c, "pass_mousewheel", 0, cat1);
    CLASS_ATTR_CATEGORY(c, "pass_focusgained", 0, cat1);
    CLASS_ATTR_CATEGORY(c, "pass_focuslost", 0, cat1);
    //CLASS_ATTR_CATEGORY(c, "pass_mousedragdelta", 0, cat1);
    CLASS_ATTR_CATEGORY(c, "pass_key", 0, cat1);
    CLASS_ATTR_CATEGORY(c, "pass_keyup", 0, cat1);

    //OUTPUT
    CLASS_ATTR_CHAR(c, "out_mouseup", 0, t_UILayerDef, out_mouseup);
    CLASS_ATTR_STYLE_LABEL(c, "out_mouseup", 0, "onoff", "to outlet");
    CLASS_ATTR_SAVE(c, "out_mouseup", 0);

    CLASS_ATTR_CHAR(c, "out_mousedown", 0, t_UILayerDef, out_mousedown);
    CLASS_ATTR_STYLE_LABEL(c, "out_mousedown", 0, "onoff", "to outlet");
    CLASS_ATTR_SAVE(c, "out_mousedown", 0);

    CLASS_ATTR_CHAR(c, "out_mousedrag", 0, t_UILayerDef, out_mousedrag);
    CLASS_ATTR_STYLE_LABEL(c, "out_mousedrag", 0, "onoff", "to outlet");
    CLASS_ATTR_SAVE(c, "out_mousedrag", 0);

    CLASS_ATTR_CHAR(c, "out_mouseenter", 0, t_UILayerDef, out_mouseenter);
    CLASS_ATTR_STYLE_LABEL(c, "out_mouseenter", 0, "onoff", "to outlet");
    CLASS_ATTR_SAVE(c, "out_mouseenter", 0);

    CLASS_ATTR_CHAR(c, "out_mouseleave", 0, t_UILayerDef, out_mouseleave);
    CLASS_ATTR_STYLE_LABEL(c, "out_mouseleave", 0, "onoff", "to outlet");
    CLASS_ATTR_SAVE(c, "out_mouseleave", 0);

    CLASS_ATTR_CHAR(c, "out_mousemove", 0, t_UILayerDef, out_mousemove);
    CLASS_ATTR_STYLE_LABEL(c, "out_mousemove", 0, "onoff", "to outlet");
    CLASS_ATTR_SAVE(c, "out_mousemove", 0);

    CLASS_ATTR_CHAR(c, "out_mousedoubleclick", 0, t_UILayerDef, out_mousedoubleclick);
    CLASS_ATTR_STYLE_LABEL(c, "out_mousedoubleclick", 0, "onoff", "to outlet");
    CLASS_ATTR_SAVE(c, "out_mousedoubleclick", 0);

    CLASS_ATTR_CHAR(c, "out_mousewheel", 0, t_UILayerDef, out_mousewheel);
    CLASS_ATTR_STYLE_LABEL(c, "out_mousewheel", 0, "onoff", "to outlet");
    CLASS_ATTR_SAVE(c, "out_mousewheel", 0);

    CLASS_ATTR_CHAR(c, "out_focusgained", 0, t_UILayerDef, out_focusgained);
    CLASS_ATTR_STYLE_LABEL(c, "out_focusgained", 0, "onoff", "to outlet");
    CLASS_ATTR_SAVE(c, "out_focusgained", 0);

    CLASS_ATTR_CHAR(c, "out_focuslost", 0, t_UILayerDef, out_focuslost);
    CLASS_ATTR_STYLE_LABEL(c, "out_focuslost", 0, "onoff", "to outlet");
    CLASS_ATTR_SAVE(c, "out_focuslost", 0);

    CLASS_ATTR_CHAR(c, "out_mousedragdelta", 0, t_UILayerDef, out_mousedragdelta);
    CLASS_ATTR_STYLE_LABEL(c, "out_mousedragdelta", 0, "onoff", "to outlet");
    CLASS_ATTR_SAVE(c, "out_mousedragdelta", 0);

    CLASS_ATTR_CHAR(c, "out_key", 0, t_UILayerDef, out_key);
    CLASS_ATTR_STYLE_LABEL(c, "out_key", 0, "onoff", "to outlet");
    CLASS_ATTR_SAVE(c, "out_key", 0);

    CLASS_ATTR_CHAR(c, "out_keyup", 0, t_UILayerDef, out_keyup);
    CLASS_ATTR_STYLE_LABEL(c, "out_keyup", 0, "onoff", "to outlet");
    CLASS_ATTR_SAVE(c, "out_keyup", 0);

    CLASS_ATTR_CATEGORY(c, "out_mousedown", 0, cat2);
    CLASS_ATTR_CATEGORY(c, "out_mousedrag", 0, cat2);
    CLASS_ATTR_CATEGORY(c, "out_mouseup", 0, cat2);
    CLASS_ATTR_CATEGORY(c, "out_mouseenter", 0, cat2);
    CLASS_ATTR_CATEGORY(c, "out_mouseleave", 0, cat2);
    CLASS_ATTR_CATEGORY(c, "out_mousemove", 0, cat2);
    CLASS_ATTR_CATEGORY(c, "out_mousedoubleclick", 0, cat2);
    CLASS_ATTR_CATEGORY(c, "out_mousewheel", 0, cat2);
    CLASS_ATTR_CATEGORY(c, "out_focusgained", 0, cat2);
    CLASS_ATTR_CATEGORY(c, "out_focuslost", 0, cat2);
    CLASS_ATTR_CATEGORY(c, "out_mousedragdelta", 0, cat2);
    CLASS_ATTR_CATEGORY(c, "out_key", 0, cat2);
    CLASS_ATTR_CATEGORY(c, "out_keyup", 0, cat2);

    // OPTIONS
    

    CLASS_ATTR_CHAR(c, "ignorewheel", 0, t_UILayerDef, ignorewheel);
    CLASS_ATTR_ACCESSORS(c, "ignorewheel", (method)NULL, (method)UILayerDef_setattr_ignorewheel);
    CLASS_ATTR_STYLE_LABEL(c, "ignorewheel", 0, "onoff", "ignore mousewheel input");
    CLASS_ATTR_SAVE(c, "ignorewheel", 0);


    CLASS_ATTR_CHAR(c, "draghide", 0, t_UILayerDef, draghide);
    CLASS_ATTR_ACCESSORS(c, "draghide", (method)NULL, (method)UILayerDef_setattr_draghide);
    CLASS_ATTR_STYLE_LABEL(c, "draghide", 0, "onoff", "hide and freeze cursor while dragging");
    CLASS_ATTR_SAVE(c, "draghide", 0);

    CLASS_ATTR_CHAR(c, "dragdelta", 0, t_UILayerDef, dragdelta);
    CLASS_ATTR_STYLE_LABEL(c, "dragdelta", 0, "onoff", "pass mousedragdelta instead of mousedrag");
    CLASS_ATTR_SAVE(c, "dragdelta", 0);

    CLASS_ATTR_CHAR(c, "wheel_delta_clip", 0, t_UILayerDef, wheel_delta_clip);
    CLASS_ATTR_STYLE_LABEL(c, "wheel_delta_clip", 0, "onoff", "mouse wheel delta output reduced to -1 and 1");
    CLASS_ATTR_SAVE(c, "wheel_delta_clip", 0);

    CLASS_ATTR_CHAR(c, "force_shift_drag", 0, t_UILayerDef, force_shift_drag);
    CLASS_ATTR_STYLE_LABEL(c, "force_shift_drag", 0, "onoff", "simulate pressed shift key when dragging");
    CLASS_ATTR_SAVE(c, "force_shift_drag", 0);

    /*CLASS_ATTR_CHAR(c, "coord_origin", 0, t_UILayerDef, coord_origin);
    CLASS_ATTR_STYLE_LABEL(c, "coord_origin", 0, "enumindex", "origin of the coordinates output");
    CLASS_ATTR_ENUMINDEX(c, "coord_origin", 0, "0Ê(box) 1Ê(patcher) 2Ê(screen)");
    CLASS_ATTR_SAVE(c, "coord_origin", 0);*/

    
    CLASS_ATTR_CATEGORY(c, "draghide", 0, cat3);
    CLASS_ATTR_CATEGORY(c, "dragdelta", 0, cat3);
    CLASS_ATTR_CATEGORY(c, "ignorewheel", 0, cat3);
    CLASS_ATTR_CATEGORY(c, "wheel_delta_clip", 0, cat3);
    CLASS_ATTR_CATEGORY(c, "force_shift_drag", 0, cat3);
    

    CLASS_ATTR_ORDER(c, "pass_mouseup", 0, "3");
    CLASS_ATTR_ORDER(c, "pass_mousedown", 0, "4");
    CLASS_ATTR_ORDER(c, "pass_mousedrag", 0, "5");
    CLASS_ATTR_ORDER(c, "pass_mouseenter", 0, "6");
    CLASS_ATTR_ORDER(c, "pass_mouseleave", 0, "7");
    CLASS_ATTR_ORDER(c, "pass_mousemove", 0, "8");
    CLASS_ATTR_ORDER(c, "pass_mousedoubleclick", 0, "9");
    CLASS_ATTR_ORDER(c, "pass_mousewheel", 0, "10");
    CLASS_ATTR_ORDER(c, "pass_focusgained", 0, "11");
    CLASS_ATTR_ORDER(c, "pass_focuslost", 0, "12");
    //CLASS_ATTR_ORDER(c, "pass_mousedragdelta", 0, "13");
    CLASS_ATTR_ORDER(c, "pass_key", 0, "14");
    CLASS_ATTR_ORDER(c, "pass_keyup", 0, "15");

    CLASS_ATTR_ORDER(c, "out_mouseup", 0, "16");
    CLASS_ATTR_ORDER(c, "out_mousedown", 0, "17");
    CLASS_ATTR_ORDER(c, "out_mousedrag", 0, "18");
    CLASS_ATTR_ORDER(c, "out_mouseenter", 0, "19");
    CLASS_ATTR_ORDER(c, "out_mouseleave", 0, "20");
    CLASS_ATTR_ORDER(c, "out_mousemove", 0, "21");
    CLASS_ATTR_ORDER(c, "out_mousedoubleclick", 0, "22");
    CLASS_ATTR_ORDER(c, "out_mousewheel", 0, "23");
    CLASS_ATTR_ORDER(c, "out_focusgained", 0, "24");
    CLASS_ATTR_ORDER(c, "out_focuslost", 0, "25");
    CLASS_ATTR_ORDER(c, "out_mousedragdelta", 0, "26");
    CLASS_ATTR_ORDER(c, "out_key", 0, "27");
    CLASS_ATTR_ORDER(c, "out_keyup", 0, "28");

    CLASS_ATTR_ORDER(c, "draghide", 0, "31");
    CLASS_ATTR_ORDER(c, "dragdelta", 0, "32");
    CLASS_ATTR_ORDER(c, "force_shift_drag", 0, "33");
    CLASS_ATTR_ORDER(c, "ignorewheel", 0, "34");
    CLASS_ATTR_ORDER(c, "wheel_delta_clip", 0, "35");
    
    
    
    CLASS_ATTR_DEFAULT(c, "patching_rect", 0, "0. 0. 50. 30.");


    class_register(CLASS_BOX, c);
    s_UILayerDef_class = c;

    _sym_keyup = gensym("keyup");
    _sym_mt_mousedrag = gensym("mt_mousedrag");
    _sym_mt_mousedragdelta = gensym("mt_mousedragdelta");
    _sym_mt_mousedown = gensym("mt_mousedown");
    _sym_mt_mouseup = gensym("mt_mouseup");
    _sym_mt_mouseenter = gensym("mt_mouseenter");
    _sym_mt_mouseleave = gensym("mt_mouseleave");
    _sym_setcursorpos_patcher = gensym("setcursorpos_patcher");
    _sym_setcursorpos_box = gensym("setcursorpos_box");
    _sym_setcursorpos_screen = gensym("setcursorpos_screen");
    _sym_setcursorpos_targetbox = gensym("setcursorpos_targetbox");
    _sym_hidecursor = gensym("hidecursor");
    _sym_showcursor = gensym("showcursor");
    _sym_cursor_patcher = gensym("cursor_patcher");
    _sym_cursor_all = gensym("cursor_all");
    _sym_cursor_objects = gensym("cursor_objects");
    _sym_cursor_target = gensym("cursor_target");
    _sym_ignorewheel = gensym("ignorewheel");
    _sym_isinlive = gensym("isinlive");

    object_post(NULL, "11UILayer 2023/09/23 11OLSEN.DE");
}


#pragma mark NOTIFY
t_max_err UILayerDef_notify(t_UILayerDef* x, t_symbol* s, t_symbol* msg, void* sender, void* data)
{
    if (DEBUG)post("notify msg: %s  sender class: %s", msg->s_name, object_classname(sender)->s_name);
    
    if (sender == x->targetBox)
    {
        if (msg == _sym_attr_modified) // a change is gonna happen
        {
            t_symbol* name = (t_symbol*)object_method((t_object*)data, _sym_getname);
            if (name == _sym_varname)
                msg = _sym_free;
            if (DEBUG)post("modified attr name: %s", name->s_name);
        }

        if (msg == _sym_free)
        {
            if (DEBUG)post(" 'free' notification");
            object_detach_byptr((t_object*)x, (t_object*)sender);

            // look again
            defer_low(x, (method)UILayerDef_bang, 0L, 0, 0L);
        }
    }
}




void UILayerDef_bang(t_UILayerDef* x)
{
    x->targetBox = NULL;
    if (x->targetname != _sym_nothing)
    {
        x->targetBox = (t_object*)object_method(x->jp, _sym_getnamedbox, x->targetname);
        //x->targetBox = jbox_get_textfield(x->targetBox);
        if (!x->targetBox)
        {
            //x->targetname = _sym_nothing;
            object_warn((t_object*)x, "there's currently no object with the scripting-name: %s ", x->targetname->s_name);
            return;
        }
        
        // get notifications from target object
        object_attach_byptr_register(x, x->targetBox, _sym_nobox);
            
        if (zgetfn((t_object*)x->targetBox, _sym_mt_mousedown) || zgetfn((t_object*)x->targetBox, _sym_mt_mousedrag))
        {
            if (DEBUG) post("target is multitouch enabled");
            x->targetmt = 1; //
        }
        else x->targetmt = 0;
        
        if (object_method(x->jp, _sym_isinlive))
        {
            if (DEBUG) post("we are in Ableton Live context");
            x->isinlive = 1;
        }
        else
            x->isinlive = 0;

        /*if (object_parameter_is_parameter(x->targetBox) )
            if (DEBUG) post("target is a parameter");

        if (object_parameter_is_in_Live(x->targetBox) )
            if (DEBUG) post("target is in Live");*/
            
        
        //x->targetdragmode = 0;
        //post("jbox understanding mousedrag %li", object_method((t_object*)x->targetBox, _sym_understands,_sym_mousedrag));
        //if (object_mess((t_object*)x->targetBox, _sym_mousedrag) || object_mess((t_object*)x->targetBox, _sym_mt_mousedrag))
        //{
        //    x->targetdragmode += 1; //
        //    if (DEBUG) post("found mousedrag method");
        //}
        //else if (object_mess((t_object*)x->targetBox, _sym_mousedragdelta) || object_mess((t_object*)x->targetBox, _sym_mt_mousedragdelta))
        //{
        //    x->targetdragmode += 2; //
        //    if (DEBUG) post("found mousedragDELTA method");
        //}
        //else
        //{
        //    //x->targetdragmode = -1; //
        //    if (DEBUG) post("found no drag method at all");
        //}
    }
}




void UILayerDef_mousedown(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers)
{
    if (DEBUG) post("UILayer_mousedown mods: %i", modifiers);

    if (!x->targetBox && x->targetname != _sym_nothing && x->pass_mousedown)
    {
        // we have a target name , want to pass, but are not connected to an object
        // this happens if the target object is deleted for example. In this case we keep the name (maybe user is)
        // the following tries to reconnect or returns. this piece is in most functions 
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }


    x->prevdragpos.x = pt.x;
    x->prevdragpos.y = pt.y;

    if (x->out_mousedown)
    {
        outlet_int(x->u_out4, modifiers);
        outlet_float(x->u_out3, pt.y);
        outlet_float(x->u_out2, pt.x);
        outlet_int(x->u_out1, 1);
    }

    if (x->targetBox && x->pass_mousedown)
    {
        if (x->targetmt)
        {
            t_mouseevent me;
            me.type = 1;
            me.index = 0;
            me.position.x = pt.x; me.position.y = pt.y;
            me.modifiers = modifiers;
            object_method((t_object*)x->targetBox,_sym_mt_mousedown , patcherview, &me);
        }
        else
            object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mousedown, patcherview, pt, modifiers);
            

        if (x->isinlive) // Special for Live: defered focus and right click context menu
        {
            if (modifiers & (1 << 5))
            {
                //post("right mouse down");
                object_method((t_object*)x->targetBox, gensym("boxcontextmenu"), patcherview);
            }
                
            t_atom a[1];
            atom_setobj(a, patcherview);
            defer_low(x, (method)UILayerDef_do_focus, NULL, 1, a);
        }
    }
}

void UILayerDef_do_focus(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
{
    //object_parameter_wants_focus(x->targetBox);
    object_method((t_object*)x->targetBox, _sym_focusgained, atom_getobj(argv) );
}


void UILayerDef_mousedrag(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers)
{
    if (DEBUG) post("UILayer_mousedrag ");

    if (!x->targetBox && x->targetname != _sym_nothing && x->pass_mousedrag)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }

    if (x->dragdelta == 1 || x->out_mousedragdelta) // if one is true, we need to man. calc dragdelta values
    {
        x->prevdragpos.x = pt.x - x->prevdragpos.x;
        x->prevdragpos.y = pt.y - x->prevdragpos.y;
    }

    if (x->out_mousedrag)
    {
        outlet_int(x->u_out4, modifiers);
        outlet_float(x->u_out3, pt.y);
        outlet_float(x->u_out2, pt.x);
        outlet_int(x->u_out1, 2);
    }
    if (x->out_mousedragdelta)
    {
        outlet_int(x->u_out4, modifiers);
        outlet_float(x->u_out3, x->prevdragpos.y);
        outlet_float(x->u_out2, x->prevdragpos.x);
        outlet_int(x->u_out1, 10);
    }

    if (x->targetBox && x->pass_mousedrag)
    {
        if (x->force_shift_drag)
        {
            modifiers |= 1UL << 1;
        }

        if (x->dragdelta == 0)
        {
            if (x->targetmt)
            {
                t_mouseevent me;
                me.type = 1;
                me.index = 0;
                me.position.x = pt.x; me.position.y = pt.y;
                me.modifiers = modifiers;
                object_method((t_object*)x->targetBox, _sym_mt_mousedrag, patcherview, &me);
            }
            else
                object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mousedrag, patcherview, pt, modifiers);
                
        }
            
        else if (x->dragdelta == 1)
        {
            object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mousedragdelta, patcherview, x->prevdragpos, modifiers);
            
        }
        
    }
        

    if (x->dragdelta == 1 || x->out_mousedragdelta) // if one is true, we need to remember drag pos
    {
        x->prevdragpos.x = pt.x;
        x->prevdragpos.y = pt.y;
    }
}

void UILayerDef_mousedragdelta(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers)
{
    if (DEBUG) post("UILayer_mousedragdelta ");

    if (!x->targetBox && x->targetname != _sym_nothing && x->pass_mousedrag)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }

    if (x->dragdelta == 0 || x->out_mousedrag) // if one is true, we need to man. calc dragdelta values
    {
        x->prevdragpos.x += pt.x;
        x->prevdragpos.y += pt.y;
    }

    if (x->out_mousedrag)
    {
        outlet_int(x->u_out4, modifiers);
        outlet_float(x->u_out3, x->prevdragpos.y);
        outlet_float(x->u_out2, x->prevdragpos.x);
        outlet_int(x->u_out1, 2);
    }
    if (x->out_mousedragdelta)
    {
        outlet_int(x->u_out4, modifiers);
        outlet_float(x->u_out3, pt.y);
        outlet_float(x->u_out2, pt.x);
        outlet_int(x->u_out1, 10);
    }

    if (x->targetBox && x->pass_mousedrag)
    {
        if (x->force_shift_drag)
        {
            modifiers |= 1UL << 1;
        }

        if (x->dragdelta == 0)
        {
            if (x->targetmt)
            {
                t_mouseevent me;
                me.type = 1;
                me.index = 0;
                me.position.x = x->prevdragpos.x;
                me.position.y = x->prevdragpos.y;
                me.modifiers = modifiers;
                object_method((t_object*)x->targetBox, _sym_mt_mousedrag, patcherview, &me);
            }
            else
                object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mousedrag, patcherview, x->prevdragpos, modifiers);
                
        }
        if (x->dragdelta == 1)
        {
            object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mousedragdelta, patcherview, pt, modifiers);
            
        }
    }
}


void UILayerDef_mouseup(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers)
{
    if (DEBUG) post("UILayer_mouseup ");

    if (!x->targetBox && x->targetname != _sym_nothing && x->pass_mouseup)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }

    if (!x->dragdelta && x->draghide) // we need to use coords where the virtual mousedrag ended
    {
        pt.x = x->prevdragpos.x;
        pt.y = x->prevdragpos.y;
    }

    if (x->out_mouseup)
    {
        outlet_int(x->u_out4, modifiers);
        outlet_float(x->u_out3, pt.y);
        outlet_float(x->u_out2, pt.x);
        outlet_int(x->u_out1, 0);
    }
    
    if (x->targetBox && x->pass_mouseup)
    {
        if (x->targetmt)
        {
            t_mouseevent me;
            me.type = 1;
            me.index = 0;
            me.position.x = pt.x;
            me.position.y = pt.y;
            me.modifiers = modifiers;
            object_method((t_object*)x->targetBox, _sym_mt_mouseup, patcherview, &me);
        }
        else
            object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mouseup, patcherview, pt, modifiers);
            
    }
}


void UILayerDef_mouseenter(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers)
{
    if (DEBUG) post("UILayer_mouseenter ");

    if (!x->targetBox && x->targetname != _sym_nothing && x->pass_mouseenter)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }

    if (x->out_mouseenter)
    {
        outlet_int(x->u_out4, modifiers);
        outlet_float(x->u_out3, pt.y);
        outlet_float(x->u_out2, pt.x);
        outlet_int(x->u_out1, 3);
    }

    if (x->targetBox && x->pass_mouseenter)
    {
        if (x->targetmt)
        {
            t_mouseevent me;
            me.type = 1;
            me.index = 0;
            me.position.x = pt.x;
            me.position.y = pt.y;
            me.modifiers = modifiers;
            object_method((t_object*)x->targetBox, _sym_mt_mouseenter, patcherview, &me);
        }
        else
            object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mouseenter, patcherview, pt, modifiers);
            
    }
        
    
    x->prevmousemove.x = pt.x;
    x->prevmousemove.y = pt.y;
}

void UILayerDef_mouseleave(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers)
{
    if (DEBUG) post("UILayer_mouseleave ");

    if (!x->targetBox && x->targetname != _sym_nothing && x->pass_mouseleave)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }

    if (x->out_mouseleave)
    {
        outlet_int(x->u_out4, modifiers);
        outlet_float(x->u_out3, pt.y);
        outlet_float(x->u_out2, pt.x);
        outlet_int(x->u_out1, 4);
    }

    if (x->targetBox && x->pass_mouseleave)
    {
        if (x->targetmt)
        {
            t_mouseevent me;
            me.type = 1;
            me.index = 0;
            me.position.x = pt.x;
            me.position.y = pt.y;
            me.modifiers = modifiers;
            object_method((t_object*)x->targetBox, _sym_mt_mouseleave, patcherview, &me);
        }
        else
            object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mouseleave, patcherview, pt, modifiers);
            
    }
    
}


void UILayerDef_mousemove(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers)
{
    // mousemove is reported constantly when cursor's on over box, even if mouse is not moving 

    if (x->out_mousemove)
    {
        if (pt.x != x->prevmousemove.x || pt.y != x->prevmousemove.y)
        {
            if (DEBUG) post("UILayer_mousemove ");
            x->prevmousemove.x = pt.x;
            x->prevmousemove.y = pt.y;

            outlet_int(x->u_out4, modifiers);
            outlet_float(x->u_out3, pt.y);
            outlet_float(x->u_out2, pt.x);
            outlet_int(x->u_out1, 5);
        }
    }

    if (x->targetBox && x->pass_mousemove)
        object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mousemove, patcherview, pt, modifiers);
        
}


void UILayerDef_mousewheel(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers, double x_inc, double y_inc)
{
    //t_atom atoms[6];
    double clipx; double clipy;
    if (DEBUG) post("UILayer_mousewheel "); // %x lf% lf% li% lf% lf", patcherview, pt.x, pt.y, modifiers, x_inc, y_inc);
    
    if (!x->targetBox && x->targetname != _sym_nothing && x->pass_mousewheel)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }

    if (x->wheel_delta_clip)
    {
        if (y_inc > 0.0)
            clipy = 1;
        else if (y_inc < 0.0)
            clipy = -1;
        else clipy = 0;

        if (x_inc > 0.0)
            clipx = 1;
        else if (x_inc < 0.0)
            clipx = -1;
        else clipx = 0;
    }
    else
    {
        clipx = x_inc;
        clipy = y_inc;
    }

    if (x->out_mousewheel)
    {
        outlet_float(x->u_out6, clipy);
        outlet_float(x->u_out5, clipx);
        outlet_int(x->u_out4, modifiers);
        outlet_float(x->u_out3, pt.y);
        outlet_float(x->u_out2, pt.x);
        outlet_int(x->u_out1, 6);
    }

    if (x->targetBox && x->pass_mousewheel)
    {
         
        object_method_direct(void*, (t_object*, t_object*, t_pt, long, double, double), (t_object*)x->targetBox, _sym_mousewheel, patcherview, pt, modifiers, clipx, clipy);
        
    }
}

void UILayerDef_mousedoubleclick(t_UILayerDef* x, t_object* patcherview, t_pt pt, long modifiers)
{
    if (DEBUG) post("UILayer_mousedoubleclick ");

    if (!x->targetBox && x->targetname != _sym_nothing && x->pass_mousedoubleclick)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }

    if (x->out_mousedoubleclick)
    {
        outlet_int(x->u_out4, modifiers);
        outlet_float(x->u_out3, pt.y);
        outlet_float(x->u_out2, pt.x);
        outlet_int(x->u_out1, 7);
    }

    if (x->targetBox && x->pass_mousedoubleclick)
        object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mousedoubleclick, patcherview, pt, modifiers);
        
}


void UILayerDef_focusgained(t_UILayerDef* x, t_object* patcherview)
{
    if (DEBUG) post("UILayer_focusgained ");

    if (!x->targetBox && x->targetname != _sym_nothing && x->pass_focusgained)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }

    if (x->out_focusgained)
    {
        outlet_int(x->u_out1, 8);
    }
    if (x->targetBox && x->pass_focusgained)
    {
        object_method((t_object*)x->targetBox, _sym_focusgained, patcherview);
        
    }
}

void UILayerDef_focuslost(t_UILayerDef* x, t_object* patcherview)
{
    if (DEBUG) post("UILayer_focuslost ");

    if (!x->targetBox && x->targetname != _sym_nothing && x->pass_focuslost)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }

    if (x->out_focuslost)
    {
        outlet_int(x->u_out1, 9);
    }
    if (x->targetBox && x->pass_focuslost)
        object_method((t_object*)x->targetBox, _sym_focuslost, patcherview);
}



long UILayerDef_key(t_UILayerDef* x, t_object* patcherview, long keycode, long modifiers, long textcharacter)
{
    if (DEBUG) post("UILayer_key ");

    if (!x->targetBox && x->targetname != _sym_nothing && x->pass_key)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }

    if (x->out_key)
    {
        outlet_int(x->u_out4, modifiers);
        outlet_float(x->u_out3, textcharacter);
        outlet_float(x->u_out2, keycode);
        outlet_int(x->u_out1, 11);
    }
    if (x->targetBox && x->pass_key)
        object_method((t_object*)x->targetBox, _sym_key, patcherview, keycode, modifiers, textcharacter);
}

long UILayerDef_keyup(t_UILayerDef* x, t_object* patcherview, long keycode, long modifiers, long textcharacter)
{
    if (DEBUG) post("UILayer_keyup ");
    if (!x->targetBox && x->targetname != _sym_nothing && x->pass_keyup)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }

    if (x->out_keyup)
    {
        outlet_int(x->u_out4, modifiers);
        outlet_float(x->u_out3, textcharacter);
        outlet_float(x->u_out2, keycode);
        outlet_int(x->u_out1, 12);
    }
    if (x->targetBox && x->pass_keyup)
        object_method((t_object*)x->targetBox, _sym_keyup, patcherview, keycode, modifiers, textcharacter);
}



// MSG FROM PATCHER
void UILayerDef_cursor(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
{
    defer(x, (method)UILayerDef_do_cursor, s, argc, argv);
}
void UILayerDef_do_cursor(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
{
    if (s == _sym_hidecursor)
    {
        #ifdef WIN_VERSION
        ShowCursor(FALSE);
        #else
        object_method(_sym_max->s_thing, _sym_hidecursor);
        #endif
    }
    else if (s == _sym_showcursor)
    {
        #ifdef WIN_VERSION
        ShowCursor(TRUE);
        #else
        object_method(_sym_max->s_thing, _sym_showcursor);
        #endif
    }
    
    else
    {
        if (!argc) return;

        if (atom_gettype(argv) == A_SYM) //it is a filename
        {
            int res;
            char filename[MAX_FILENAME_CHARS];
            t_uint32  type_searched_for = NULL;
            t_uint32  type_found;
            short path_id = 0;
            t_symbol* theSym = atom_getsym(argv);
            int hotspot_x = 0;
            int hotspot_y = 0;
            strncpy_zero(filename, theSym->s_name, MAX_FILENAME_CHARS);

            type_searched_for = NULL;
            
            res = locatefile_extended(filename, &path_id, &type_found, NULL, 0);

            /*if (res == 0 && path_id != 0) { post("filename found"); }
            else */if (res != 0) { object_error((t_object*)x, "can't find the image file"); return; }

            t_jsurface* icon1 = NULL;
            icon1 = jgraphics_image_surface_create_from_file(filename, path_id);
            if (argc > 2)
            {
                hotspot_x = atom_getlong(argv+1);
                hotspot_y = atom_getlong(argv+2);
            }
            

            if (s == _sym_cursor_patcher)
            {
                t_object* pv = NULL;

                if (argc > 3)
                    pv = object_method(x->jp, _sym_getnthview, atom_getlong(argv + 3));
                else
                    pv = jpatcher_get_firstview(x->jp);

                if (!pv) {
                    object_error((t_object*)x, "problem getting the patcherview");
                    return;
                }
                jmouse_setcursor_surface(pv, NULL, icon1, hotspot_x, hotspot_y);
                
            }
            else if (s == _sym_cursor_objects)
            {
                t_object* jb = jpatcher_get_firstobject(x->jp);
                while (jb)
                {
                    jmouse_setcursor_surface(NULL, jb, icon1, hotspot_x, hotspot_y);
                    jb = jbox_get_nextobject(jb);
                }
            }
            else if (s == _sym_cursor_all)
            {
                t_object* pv = NULL;

                if (argc > 3)
                    pv = object_method(x->jp, _sym_getnthview, atom_getlong(argv + 3));
                else
                    pv = jpatcher_get_firstview(x->jp);

                if (!pv) {
                    object_error((t_object*)x, "problem getting the patcherview");
                    return;
                }
                jmouse_setcursor_surface(pv, NULL, icon1, hotspot_x, hotspot_y);

                t_object* jb = jpatcher_get_firstobject(x->jp);
                while (jb)
                {
                    jmouse_setcursor_surface(NULL, jb, icon1, hotspot_x, hotspot_y);
                    jb = jbox_get_nextobject(jb);
                }
            }
            else if (s == _sym_cursor_target)
            {
                if (x->targetBox)
                {
                    jmouse_setcursor_surface(NULL, x->targetBox, icon1, hotspot_x, hotspot_y);
                }
                else
                {
                    object_error((t_object*)x, "there's no target object connected");
                }
                
            }
            else
            {
                jmouse_setcursor_surface(NULL, x->jb, icon1, hotspot_x, hotspot_y);
            }


            jgraphics_surface_destroy(icon1);

            ;
        }
        else
        {
            long curNum = atom_getlong(argv);

            if (s == _sym_cursor_patcher)
            {
                t_object* pv = NULL;
                
                if (argc > 3)
                    pv = object_method(x->jp, _sym_getnthview, atom_getlong(argv + 3));
                else
                    pv = jpatcher_get_firstview(x->jp);

                if (!pv) {
                    object_error((t_object*)x, "problem getting the patcherview");
                    return;
                }
                jmouse_setcursor(pv, NULL, curNum);
            }
            else if (s == _sym_cursor_objects)
            {
                t_object* jb = jpatcher_get_firstobject(x->jp);
                while (jb)
                {
                    jmouse_setcursor(NULL, jb, curNum);
                    jb = jbox_get_nextobject(jb);
                }
            }
            else if (s == _sym_cursor_all)
            {
                t_object* pv = NULL;

                if (argc > 3)
                    pv = object_method(x->jp, _sym_getnthview, atom_getlong(argv + 3));
                else
                    pv = jpatcher_get_firstview(x->jp);

                if (!pv) {
                    object_error((t_object*)x, "problem getting the patcherview");
                    return;
                }
                jmouse_setcursor(pv, NULL, curNum);

                t_object* jb = jpatcher_get_firstobject(x->jp);
                while (jb)
                {
                    jmouse_setcursor(NULL, jb, curNum);
                    jb = jbox_get_nextobject(jb);
                }
            }
            else if (s == _sym_cursor_target)
            {
                if (x->targetBox)
                {
                    jmouse_setcursor(NULL, x->targetBox, curNum); 
                }
                else
                {
                    object_error((t_object*)x, "there's no target object connected");
                }

            }
            else
            {
                jmouse_setcursor(NULL, x->jb, curNum);
            }
            
        }

    }

    

    //object_method(x->w_patcher, _sym_refresh);
}


void UILayerDef_addtovalue(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
{
    int i;
    long acount = 0;
    t_atom* as = NULL;
    double newfloat;
    long newint;
    
    if (!x->targetBox && x->targetname != _sym_nothing)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }

    if (x->targetBox)
    {
        object_getvalueof(x->targetBox, &acount, &as);

        if (acount)
        {
            for (i = 0; i < acount; i++)
            {
                if (argc >= (i + 1)) // if we have a fitting input argument
                {
                    if (atom_gettype(as + i) == A_LONG)
                    {
                        newint = atom_getlong(as + i) + atom_getlong(argv + i);
                        atom_setlong(as + i, newint);
                    }
                    else if (atom_gettype(as+i) == A_FLOAT)
                    {
                        newfloat = atom_getfloat(as+i) + atom_getfloat(argv+i);
                        atom_setfloat(as+i, newfloat);
                    }
                    else
                    {
                        ;
                        //object_error((t_object*)x, "the target object did not return an int or float value");
                    }
                }
    
            }
            object_setvalueof(x->targetBox, acount, as);
            sysmem_freeptr(as);
        }
        else
        {
            object_error((t_object*)x, "the target object does not support getting its value");
        }
       
    }
    else
    {
        object_error((t_object*)x, "there's no target object specified");
    }
}

void UILayerDef_setvalue(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
{
    if (!x->targetBox && x->targetname != _sym_nothing)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }

    if (x->targetBox)
    {
        t_max_err err = object_setvalueof(x->targetBox, argc, argv);
            
        if (err)
            object_error((t_object*)x, "error setting value of the target object (it needs to support the setvalueof interface)");
    }
    else
    {
        object_error((t_object*)x, "there's no target object available");
    }
}

void UILayerDef_setcursorpos(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
{
    t_object* pv = NULL;
    if (argc < 2) {
        object_error((t_object*)x, "setcursor: missing args (x,y)");
        return;
    }
    if (argc >= 3)
        pv = object_method(x->jp, _sym_getnthview, atom_getlong(argv + 2));
    else
        pv = jpatcher_get_firstview(x->jp);

    if (!pv) {
        object_error((t_object*)x, "problem getting the patcherview");
        return;
    }

    if (s == _sym_setcursorpos_box)
    {
        jmouse_setposition_box(pv, x->jb, atom_getfloat(argv), atom_getfloat(argv + 1));
    }
    else if (s == _sym_setcursorpos_patcher)
    {
        jmouse_setposition_view(pv, atom_getfloat(argv), atom_getfloat(argv + 1));
    }
    else if (s == _sym_setcursorpos_screen)
    {
        jmouse_setposition_global(atom_getlong(argv), atom_getlong(argv + 1));
    }
    else if (s == _sym_setcursorpos_targetbox)
    {
        if (!x->targetBox && x->targetname != _sym_nothing)
        {
            
            UILayerDef_bang(x);
            if (!x->targetBox) return;
        }

        if (x->targetBox)
        {
            jmouse_setposition_box(pv, x->targetBox, atom_getfloat(argv), atom_getfloat(argv + 1));
        }
        else
        {
            object_error((t_object*)x, "there's no target object available");
        }
    }
}


//void UILayerDef_send(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
//{
//    defer(x, (method)UILayerDef_do_send, s, argc, argv);
//}
void UILayerDef_send(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
{

    if (!argc)
    {
        object_error((t_object*)x, "missing args");
        return;
    }

    if (!x->targetBox && x->targetname != _sym_nothing)
    {
        
        UILayerDef_bang(x);
        if (!x->targetBox) return;
    }
    
    if (DEBUG) post("send %s", atom_getsym(argv)->s_name);

    t_symbol* event = atom_getsym(argv);
    

    if (event == _sym_mousedown)
    {
        t_pt mypt;
        long modifiers = 0;
        mypt.x = argc >= 2 ? atom_getfloat(argv + 1) : 0;
        mypt.y = argc >= 3 ? atom_getfloat(argv + 2) : 0;
        modifiers = argc >= 4 ? atom_getlong(argv + 3) : 0;
        object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mousedown, jpatcher_get_firstview(x->jp), mypt, modifiers);
        
    }

    else if (event == _sym_mouseup)
    {
        t_pt mypt;
        long modifiers = 0;
        mypt.x = argc >= 2 ? atom_getfloat(argv + 1) : 0;
        mypt.y = argc >= 3 ? atom_getfloat(argv + 2) : 0;
        modifiers = argc >= 4 ? atom_getlong(argv + 3) : 0;
        object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mouseup, jpatcher_get_firstview(x->jp), mypt, modifiers);
        
    }

    else if (event == _sym_mousedrag)
    {
        t_pt mypt;
        long modifiers = 0;
        if (argc < 3)
        {
            object_error((t_object*)x, "missing args (drag pos)");
            return;
        }
        mypt.x = atom_getfloat(argv + 1);
        mypt.y = atom_getfloat(argv + 2);
        modifiers = argc >= 4 ? atom_getlong(argv + 3) : 0;
        object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mousedrag, jpatcher_get_firstview(x->jp), mypt, modifiers);
        
    }
    else if (event == _sym_mouseenter)
    {
        t_pt mypt;
        long modifiers = 0;

        mypt.x = argc >= 2 ? atom_getfloat(argv + 1) : 0;
        mypt.y = argc >= 3 ? atom_getfloat(argv + 2) : 0;
        modifiers = argc >= 4 ? atom_getlong(argv + 3) : 0;
        object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mouseenter, jpatcher_get_firstview(x->jp), mypt, modifiers);
        
    }
    else if (event == _sym_mouseleave)
    {
        t_pt mypt;
        long modifiers = 0;

        mypt.x = argc >= 2 ? atom_getfloat(argv + 1) : -1;
        mypt.y = argc >= 3 ? atom_getfloat(argv + 2) : -1;
        modifiers = argc >= 4 ? atom_getlong(argv + 3) : 0;
        object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mouseleave, jpatcher_get_firstview(x->jp), mypt, modifiers);
        
    }
    else if (event == _sym_mousemove)
    {
        t_pt mypt;
        long modifiers = 0;
        if (argc < 3)
        {
            object_error((t_object*)x, "missing args (move pos)");
            return;
        }
        mypt.x = atom_getfloat(argv + 1);
        mypt.y = atom_getfloat(argv + 2);
        modifiers = argc >= 4 ? atom_getlong(argv + 3) : 0;
        object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mousemove, jpatcher_get_firstview(x->jp), mypt, modifiers);
        
    }
    else if (event == _sym_mousewheel)
    {
        double x_inc; double y_inc;
        t_pt mypt;
        long modifiers = 0;
        if (argc < 6)
        {
            object_error((t_object*)x, "missing args (mousewheel pos_x pos_y mod inc_x inc_y)");
            return;
        }
        mypt.x = atom_getfloat(argv + 1);
        mypt.y = atom_getfloat(argv + 2);
        modifiers = atom_getlong(argv + 3);
        x_inc = atom_getfloat(argv + 4);
        y_inc = atom_getfloat(argv + 5);

        object_method_direct(void*, (t_object* , t_object*, t_pt, long, double, double), (t_object*)x->targetBox, _sym_mousewheel, jpatcher_get_firstview(x->jp), mypt, modifiers, x_inc, y_inc);
    }
    
    else if (event == _sym_mousedoubleclick)
    {
        t_pt mypt;
        long modifiers = 0;

        mypt.x = argc >= 2 ? atom_getfloat(argv + 1) : 0;
        mypt.y = argc >= 3 ? atom_getfloat(argv + 2) : 0;
        modifiers = argc >= 4 ? atom_getlong(argv + 3) : 0;
        object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mousedoubleclick, jpatcher_get_firstview(x->jp), mypt, modifiers);
        
    }
    else if (event == _sym_focusgained)
    {
        object_method((t_object*)x->targetBox, _sym_focusgained, jpatcher_get_firstview(x->jp));
    }
    else if (event == _sym_focuslost)
    {
        object_method((t_object*)x->targetBox, _sym_focuslost, jpatcher_get_firstview(x->jp));
    }
    else if (event == _sym_mousedragdelta)
    {
        t_pt mypt;
        long modifiers = 0;
        if (argc < 3)
        {
            object_error((t_object*)x, "missing args (drag delta)");
            return;
        }
        mypt.x = atom_getfloat(argv + 1);
        mypt.y = atom_getfloat(argv + 2);
        modifiers = argc >= 4 ? atom_getlong(argv + 3) : 0;
        object_method_direct(void*, (t_object* , t_object*, t_pt, long), (t_object*)x->targetBox, _sym_mousedragdelta, jpatcher_get_firstview(x->jp), mypt, modifiers);
        
    }
    else if (event == _sym_key)
    {
        t_pt mypt;
        long modifiers = 0;
        long keycode = 0;
        long textcharacter = 0;
        if (argc < 2)
        {
            object_error((t_object*)x, "missing args");
            return;
        }
        mypt.x = atom_getlong(argv + 1);
        mypt.y = argc >= 3 ? atom_getlong(argv + 2) : 0;
        modifiers = argc >= 4 ? atom_getlong(argv + 3) : 0;
        object_method((t_object*)x->targetBox, _sym_key, jpatcher_get_firstview(x->jp), keycode, modifiers, textcharacter);
    }
    else if (event == _sym_keyup)
    {
        t_pt mypt;
        long modifiers = 0;
        long keycode = 0;
        long textcharacter = 0;
        if (argc < 2)
        {
            object_error((t_object*)x, "missing args");
            return;
        }
        mypt.x = atom_getlong(argv + 1);
        mypt.y = argc >= 3 ? atom_getlong(argv + 2) : 0;
        modifiers = argc >= 4 ? atom_getlong(argv + 3) : 0;
        object_method((t_object*)x->targetBox, _sym_keyup, jpatcher_get_firstview(x->jp), keycode, modifiers, textcharacter);
    }
    else
    {
        object_method_typed(x->targetBox, atom_getsym(argv), argc - 1, argv + 1, NULL);
    }
}

void UILayerDef_closemenu(t_UILayerDef* x)
{
    jpopupmenu_closeall();
}

void UILayerDef_targetinfo(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
{
    object_post((t_object*)x, "Target Info:");

    if (x->targetBox)
    {
        object_post((t_object*)x, "varname: %s", jbox_get_varname(x->targetBox)->s_name);
        object_post((t_object*)x, "maxclass: %s", object_classname(x->targetBox)->s_name);

        if (zgetfn((t_object*)x->targetBox, _sym_mt_mousedown) || zgetfn((t_object*)x->targetBox, _sym_mt_mousedrag))
        {
            object_post((t_object*)x, "multitouch: YES");
        }
        else
            object_post((t_object*)x, "multitouch: NO");

        if (zgetfn((t_object*)x->targetBox, _sym_mousedrag) || zgetfn((t_object*)x->targetBox, _sym_mt_mousedrag))
        {
            object_post((t_object*)x, "default drag: YES");
        }
        else
            object_post((t_object*)x, "default drag: NO");

        if (zgetfn((t_object*)x->targetBox, _sym_mousedragdelta) || zgetfn((t_object*)x->targetBox, _sym_mt_mousedragdelta))
        {
            object_post((t_object*)x, "delta drag: YES");
        }
        else
            object_post((t_object*)x, "delta drag: NO");

        int i;
        long acount = 0;
        t_atom* as = NULL;
        double newfloat;
        long newint;
        object_getvalueof(x->targetBox, &acount, &as);

        if (acount)
        {
            long textcount = 0;
            char* valuetext;
            atom_gettext(acount, as, &textcount, &valuetext, 0);

            object_post((t_object*)x, "getvalue: %s", valuetext);

            sysmem_freeptr(valuetext);
            sysmem_freeptr(as);
        }
        else
        {
            object_error((t_object*)x, "getvalue: not supported");
        }
    }
    else
        object_post((t_object*)x, "currently not connected to a target object");
}

void UILayerDef_putontop(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
{
    t_object* pv = NULL;
    t_rect boxrect;

    if (x->targetBox)
    {
        if (argc >= 1)
            pv = object_method(x->jp, _sym_getnthview, atom_getlong(argv));
        else
            pv = jpatcher_get_firstview(x->jp);

        if (!pv) {
            object_error((t_object*)x, "problem getting the patcherview");
            return;
        }

        jbox_get_rect_for_view(x->targetBox, pv, &boxrect);
        jbox_set_rect_for_view((t_object*)x, pv, &boxrect);
        object_method((t_object*)x, _sym_bringtofront); 

    }
    else
        object_post((t_object*)x, "currently not connected to a target object");
}



// Attr setters

t_max_err UILayerDef_setattr_ignorewheel(t_UILayerDef* x, void* attr, long ac, t_atom* av)
{
    x->ignorewheel = atom_getlong(av);
    if (x->we_are_loaded)
        UILayerDef_do_ignorewheel(x, NULL, ac, av);
    else
        defer_low(x, (method)UILayerDef_do_ignorewheel, NULL, ac, av);

    return MAX_ERR_NONE;
}
void UILayerDef_do_ignorewheel(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
{
    if (DEBUG) post("ignorewheel setter ");

    if (atom_getlong(argv))
        object_deletemethod((t_object*)x, _sym_mousewheel);
    else
        object_addmethod((t_object*)x, (method)UILayerDef_mousewheel, "mousewheel", A_CANT, 0);
}


t_max_err UILayerDef_setattr_draghide(t_UILayerDef* x, void* attr, long ac, t_atom* av)
{
    x->draghide = atom_getlong(av);
    if (x->we_are_loaded)
        UILayerDef_do_draghide(x, NULL, ac, av);
    else
        defer_low(x, (method)UILayerDef_do_draghide, NULL, ac, av);
}
void UILayerDef_do_draghide(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
{
    if (DEBUG) post("draghide setter ");
    jbox_set_mousedragdelta(x->jb, atom_getlong(argv) ? 1 : 0);
}


t_max_err UILayerDef_setattr_target(t_UILayerDef* x, void* attr, long ac, t_atom* av)
{
    if (ac && av)
        atom_setsym(x->targetatom, atom_getsym(av));
    else
        atom_setsym(x->targetatom, _sym_nothing);

    x->targetname = atom_getsym(x->targetatom);

    if (x->we_are_loaded)
        UILayerDef_bang(x);
    else
        defer_low(x, (method)UILayerDef_bang, NULL, (short)0, NULL);

    return MAX_ERR_NONE;
}



void UILayerDef_assist(t_UILayerDef* x, void* b, long m, long a, char* s)
{
    
    if (m == ASSIST_INLET) sprintf(s, "In 1");
    else
    {
        switch (a)
        {
        case 0:
            sprintf(s, "event type");
            break;
        case 1:
            sprintf(s, "pos x / keycode");
            break;
        case 2:
            sprintf(s, "pos y / textcharacter");
            break;
        case 3:
            sprintf(s, "modifiers");
            break;
        case 4:
            sprintf(s, "wheel delta x");
            break;
        case 5:
            sprintf(s, "wheel delta y");
            break;
        
        }
    }
}

void UILayerDef_free(t_UILayerDef*x)
{
    if (x->targetBox)
    {
        object_detach_byptr((t_object*)x, (t_object*)x->targetBox);
    }
    
    jbox_free((t_jbox *)x);
}

void* UILayerDef_new(t_symbol *s, long argc, t_atom *argv)
{
    
    t_UILayerDef* x = NULL;
    t_dictionary *d = NULL;
    long boxflags;
    t_max_err err;

    if (!(d = object_dictionaryarg(argc,argv)))
        return NULL;

    x = (t_UILayerDef*)object_alloc(s_UILayerDef_class);

    //defaults:
    
    x->targetBox = NULL;
    x->targetname = _sym_nothing;

    x->pass_mousedown =  x->pass_mousedrag =  x->pass_mouseup =  x->pass_mouseenter = \
        x->pass_mouseleave =  x->pass_mousemove =  x->pass_mousedoubleclick =  x->pass_mousewheel = \
        x->pass_focusgained =  x->pass_focuslost =  x->pass_key  =  x->pass_keyup = 1;

    x->out_mousedown =  x->out_mousedrag =  x->out_mouseup =  x->out_mouseenter = \
        x->out_mouseleave =  x->out_mousemove =  x->out_mousedoubleclick =  x->out_mousewheel = \
        x->out_focusgained =  x->out_focuslost =  x->out_key =  x->out_mousedragdelta =  x->out_keyup = 1;

    x->prevmousemove.x = -32000;
    x->prevmousemove.y = -32000;

    x->force_shift_drag = 0;
    x->ignorewheel = 0;
    x->wheel_delta_clip = 0;
    x->targetmt = 0;
    x->we_are_loaded = 0;

    x->draghide = 0;
    x->dragdelta = 0;

    if (!dictionary_hasentry(d, _sym_ignorewheel))
        dictionary_appendlong(d, _sym_ignorewheel, 0);

    t_atom_long    modevalue = 0;
    err = dictionary_getlong(d, _sym_mode, &modevalue);
    
    x->mode = modevalue;
    if (DEBUG) post("starting object mode %li", x->mode);
    

    if (x->mode == 1)
    {
        boxflags = 0
            | JBOX_DRAWFIRSTIN
            | JBOX_NODRAWBOX
            | JBOX_DRAWINLAST
            | JBOX_TRANSPARENT
            //        | JBOX_NOGROW
            //        | JBOX_GROWY
            | JBOX_GROWBOTH
            //| JBOX_HILITE
            //        | JBOX_BACKGROUND
            //| JBOX_DRAWBACKGROUND
            //        | JBOX_NOFLOATINSPECTOR
            //        | JBOX_TEXTFIELD
            //| JBOX_MOUSEDRAGDELTA
            //        | JBOX_TEXTFIELD
            ;
    }
    else
    {
        boxflags = 0
            | JBOX_DRAWFIRSTIN
            | JBOX_NODRAWBOX
            | JBOX_DRAWINLAST
            | JBOX_TRANSPARENT
            //        | JBOX_NOGROW
            //        | JBOX_GROWY
            | JBOX_GROWBOTH
            | JBOX_HILITE
            //        | JBOX_BACKGROUND
            //| JBOX_DRAWBACKGROUND
            //        | JBOX_NOFLOATINSPECTOR
            //        | JBOX_TEXTFIELD
            //| JBOX_MOUSEDRAGDELTA
            //        | JBOX_TEXTFIELD
            ;
    }
    jbox_new((t_jbox *)x, boxflags, argc, argv);
    x->u_box.b_firstin = (void *)x;

    

    //x->u_out8 = intout((t_object*)x);
    //x->u_out7 = intout((t_object*)x);
    x->u_out6 = floatout((t_object*)x);
    x->u_out5 = floatout((t_object*)x);
    x->u_out4 = intout((t_object*)x);
    x->u_out3 = floatout((t_object*)x);
    x->u_out2 = floatout((t_object*)x);
    x->u_out1 = intout((t_object *)x);
    

    jbox_ready((t_jbox *)x);
    attr_dictionary_process(x, d);
    object_attr_setdisabled((t_object*)x, gensym("mode"), 1); // 'mode' should only be set at object creation time
    

    //get patcher & box
    err = object_obex_lookup(x, gensym("#P"), (t_object**)&x->jp);
    if (err != MAX_ERR_NONE)
    {
        object_error((t_object*)x, "failed to get the patcher containing this object");
        return NULL;
    }
    err = object_obex_lookup(x, gensym("#B"), (t_object**)&x->jb);
    if (err != MAX_ERR_NONE)
    {
        object_error((t_object*)x, "failed to get the box containing this object");
        return NULL;
    }

    //
    defer_low(x, (method)UILayerDef_loaded, s, argc, argv);

    return x;
}

void UILayerDef_loaded(t_UILayerDef* x, t_symbol* s, long argc, t_atom* argv)
{
    x->we_are_loaded = 1;
}

