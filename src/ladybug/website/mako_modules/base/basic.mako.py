# -*- encoding:ascii -*-
from mako import runtime, filters, cache
UNDEFINED = runtime.UNDEFINED
__M_dict_builtin = dict
__M_locals_builtin = locals
_magic_number = 8
_modified_time = 1354744638.610034
_enable_loop = True
_template_filename = '/Users/christopher/Documents/University Stuff/2012-Senior Project/smarthome/src/ladybug/website/mako_templates/base/basic.mako'
_template_uri = '/base/basic.mako'
_source_encoding = 'ascii'
_exports = ['displayitem', 'displayuniverse', 'title_main', u'mainbody_content']


# SOURCE LINE 3

from mirabilis.core import (RStateEntity, 
                            RWStateEntity, 
                            WStateEntity,
                            StateEntity, 
                            UNINITIALIZED, 
                            NEVER_WRITTEN)
assert UNINITIALIZED != UNDEFINED
assert NEVER_WRITTEN != UNDEFINED


def _mako_get_namespace(context, name):
    try:
        return context.namespaces[(__name__, name)]
    except KeyError:
        _mako_generate_namespaces(context)
        return context.namespaces[(__name__, name)]
def _mako_generate_namespaces(context):
    pass
def _mako_inherit(template, context):
    _mako_generate_namespaces(context)
    return runtime._inherit_from(context, u'/base.mako', _template_uri)
def render_body(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        __M_locals = __M_dict_builtin(pageargs=pageargs)
        def displayuniverse():
            return render_displayuniverse(context.locals_(__M_locals))
        def mainbody_content():
            return render_mainbody_content(context.locals_(__M_locals))
        __M_writer = context.writer()
        # SOURCE LINE 1
        __M_writer(u'\n\n')
        # SOURCE LINE 12
        __M_writer(u'\n\n\n')
        # SOURCE LINE 17
        __M_writer(u'\n\n')
        if 'parent' not in context._data or not hasattr(context._data['parent'], 'mainbody_content'):
            context['self'].mainbody_content(**pageargs)
        

        # SOURCE LINE 22
        __M_writer(u'\n\n\n')
        # SOURCE LINE 68
        __M_writer(u'\n\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_displayitem(context,smarthomeitem):
    __M_caller = context.caller_stack._push_frame()
    try:
        isinstance = context.get('isinstance', UNDEFINED)
        type = context.get('type', UNDEFINED)
        repr = context.get('repr', UNDEFINED)
        __M_writer = context.writer()
        # SOURCE LINE 25
        __M_writer(u'\n')
        # SOURCE LINE 27
        __M_writer(u'    <td class="smarthomeitem_body">\n        ')
        # SOURCE LINE 28
        __M_writer(filters.html_escape(unicode(repr(smarthomeitem) )))
        __M_writer(u'\n    </td>\n    <td class="smarthomeitem_state">\n')
        # SOURCE LINE 31
        if isinstance(smarthomeitem, StateEntity):
            # SOURCE LINE 32
            __M_writer(u'            ')
 
            assert UNINITIALIZED != UNDEFINED
            assert NEVER_WRITTEN != UNDEFINED
            
            if not isinstance(smarthomeitem, (RWStateEntity, WStateEntity)):
                readonly = 'disabled="disabled"'
            else:
                readonly = ""
            
            if isinstance(smarthomeitem, (RStateEntity, RWStateEntity)):
                prefix = "Current state:"
                display = smarthomeitem.read()
            elif isinstance(smarthomeitem, WStateEntity):
                prefix = "Last written value:"
                display = smarthomeitem.lastwrite
            else:
                assert False  # not reached
            
            #print UNINITIALIZED, ":", id(UNINITIALIZED)
            #print display, ":", id(display)
            #print UNINITIALIZED == display
            if display in (UNINITIALIZED, NEVER_WRITTEN):
                display = ""
            else:
                display = repr(display) + repr(type(display))
                #display = ""
                        
            
            # SOURCE LINE 58
            __M_writer(u'\n        \n            <form action="/change/" method="post">\n                ')
            # SOURCE LINE 61
            __M_writer(unicode(prefix))
            __M_writer(u'\n                <input type="hidden" name="global_id" value="')
            # SOURCE LINE 62
            __M_writer(unicode(smarthomeitem.global_id))
            __M_writer(u'" />\n                <input type="text" name="writedata" value="')
            # SOURCE LINE 63
            __M_writer(filters.html_escape(unicode(display )))
            __M_writer(u'" ')
            __M_writer(unicode(readonly))
            __M_writer(u' />\n                <input type="submit" value="Change" />\n            </form>\n')
        # SOURCE LINE 67
        __M_writer(u'    </td>\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_displayuniverse(context):
    __M_caller = context.caller_stack._push_frame()
    try:
        universe = context.get('universe', UNDEFINED)
        def displayitem(smarthomeitem):
            return render_displayitem(context,smarthomeitem)
        iter = context.get('iter', UNDEFINED)
        len = context.get('len', UNDEFINED)
        sorted = context.get('sorted', UNDEFINED)
        __M_writer = context.writer()
        # SOURCE LINE 70
        __M_writer(u'\n    <p>\n        Universe with \n        ')
        # SOURCE LINE 73
        __M_writer(unicode(len(universe.devices)))
        __M_writer(u' devices \n        and ')
        # SOURCE LINE 74
        __M_writer(unicode(len(universe.objectsdict)))
        __M_writer(u' items.\n    </p>\n    ')
        # SOURCE LINE 76
 
        #itemsbypath = {x.path: x for x in universe.objectsdict.valuesiter()}
        #sorteditems = [itemsbypath[path] 
        #               for path in sorted(itemsbypath.keys())]
        itemsbypath = {}
        objectsdict = universe.objectsdict
        for global_id in sorted(objectsdict.iterkeys()):
            smarthomeitem = objectsdict[global_id]
            path = smarthomeitem.path
            itemsbypath.setdefault(path, [])
            itemsbypath[path].append(smarthomeitem)
            
        
        # SOURCE LINE 87
        __M_writer(u'\n    <table border="1">\n')
        # SOURCE LINE 89
        for path in sorted(itemsbypath.keys()):
            # SOURCE LINE 90
            __M_writer(u'            ')
 
            assert len(itemsbypath[path])
            myiter = iter(itemsbypath[path])
                        
            
            # SOURCE LINE 93
            __M_writer(u'\n            <tr>\n                <td class="pathname" rowspan="')
            # SOURCE LINE 95
            __M_writer(unicode(len(itemsbypath[path])))
            __M_writer(u'">\n                    ')
            # SOURCE LINE 96
            __M_writer(filters.html_escape(unicode(path )))
            __M_writer(u'\n                </td>\n                ')
            # SOURCE LINE 98
            __M_writer(unicode(displayitem(myiter.next())))
            __M_writer(u'\n            </tr>\n')
            # SOURCE LINE 100
            for smarthomeitem in myiter:
                # SOURCE LINE 101
                __M_writer(u'                <tr>\n                    ')
                # SOURCE LINE 102
                __M_writer(unicode(displayitem(smarthomeitem)))
                __M_writer(u'\n                </tr>\n')
        # SOURCE LINE 106
        __M_writer(u'    </table>\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_title_main(context):
    __M_caller = context.caller_stack._push_frame()
    try:
        __M_writer = context.writer()
        # SOURCE LINE 15
        __M_writer(u'\n    universal view\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_mainbody_content(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        def displayuniverse():
            return render_displayuniverse(context)
        def mainbody_content():
            return render_mainbody_content(context)
        __M_writer = context.writer()
        # SOURCE LINE 19
        __M_writer(u'\n')
        # SOURCE LINE 21
        __M_writer(u'    ')
        __M_writer(unicode(displayuniverse()))
        __M_writer(u'\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


