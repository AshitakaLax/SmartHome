# -*- encoding:ascii -*-
from mako import runtime, filters, cache
UNDEFINED = runtime.UNDEFINED
__M_dict_builtin = dict
__M_locals_builtin = locals
_magic_number = 8
_modified_time = 1354205877.713228
_enable_loop = True
_template_filename = '/Users/christopher/Documents/University Stuff/2012-Senior Project/smarthome/src/ladybug/website/mako_templates/base/basic.mako'
_template_uri = '/base/basic.mako'
_source_encoding = 'ascii'
_exports = [u'credits', 'displayuniverse', 'title_main', u'mainbody_content']


# SOURCE LINE 3
from mirabilis.core import RStateEntity, RWStateEntity, WStateEntity 

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
        def credits():
            return render_credits(context.locals_(__M_locals))
        def displayuniverse():
            return render_displayuniverse(context.locals_(__M_locals))
        def mainbody_content():
            return render_mainbody_content(context.locals_(__M_locals))
        __M_writer = context.writer()
        # SOURCE LINE 1
        __M_writer(u'\n\n')
        # SOURCE LINE 3
        __M_writer(u'\n\n\n')
        # SOURCE LINE 8
        __M_writer(u'\n\n\n')
        if 'parent' not in context._data or not hasattr(context._data['parent'], 'mainbody_content'):
            context['self'].mainbody_content(**pageargs)
        

        # SOURCE LINE 14
        __M_writer(u'\n\n\n')
        if 'parent' not in context._data or not hasattr(context._data['parent'], 'credits'):
            context['self'].credits(**pageargs)
        

        # SOURCE LINE 29
        __M_writer(u'\n\n\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_credits(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        def credits():
            return render_credits(context)
        __M_writer = context.writer()
        # SOURCE LINE 17
        __M_writer(u'\n    Smart Home Server credits\n    <br />\n    <br />\n    Coded by Christopher Johnson\n    <br />\n    Assisted by Levi Balling, Dario Bosnjak, and Todd Rogers\n    <br />\n    <br />\n    Implemented using Python programming language and libraries\n    <br />\n    Built on Django web framework and Mako templating engine\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_displayuniverse(context):
    __M_caller = context.caller_stack._push_frame()
    try:
        universe = context.get('universe', UNDEFINED)
        isinstance = context.get('isinstance', UNDEFINED)
        repr = context.get('repr', UNDEFINED)
        len = context.get('len', UNDEFINED)
        sorted = context.get('sorted', UNDEFINED)
        __M_writer = context.writer()
        # SOURCE LINE 32
        __M_writer(u'\n    <p>\n        Universe with \n        ')
        # SOURCE LINE 35
        __M_writer(unicode(len(universe.devices)))
        __M_writer(u' devices \n        and ')
        # SOURCE LINE 36
        __M_writer(unicode(len(universe.objectsdict)))
        __M_writer(u' items.\n    </p>\n    ')
        # SOURCE LINE 38
 
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
            
        
        # SOURCE LINE 49
        __M_writer(u'\n')
        # SOURCE LINE 50
        for path in sorted(itemsbypath.keys()):
            # SOURCE LINE 51
            __M_writer(u'        <hr />\n        <div class="pathanditems">\n            <div class="pathname">\n                ')
            # SOURCE LINE 54
            __M_writer(filters.html_escape(unicode(path )))
            __M_writer(u'\n            </div>\n')
            # SOURCE LINE 56
            for smarthomeitem in itemsbypath[path]:
                # SOURCE LINE 57
                __M_writer(u'                <div class="smarthomeitem">\n                    Global ID: ')
                # SOURCE LINE 58
                __M_writer(unicode(smarthomeitem.global_id))
                __M_writer(u'\n                    <div class="smarthomeitem_body">\n                        ')
                # SOURCE LINE 60
                __M_writer(filters.html_escape(unicode(repr(smarthomeitem) )))
                __M_writer(u'\n                    </div>\n')
                # SOURCE LINE 62
                if isinstance(smarthomeitem, (RStateEntity, RWStateEntity)):
                    # SOURCE LINE 63
                    __M_writer(u'                        <div class="smarthomeitem_state">\n                            Current state: ')
                    # SOURCE LINE 64
                    __M_writer(filters.html_escape(unicode(repr(smarthomeitem.read()) )))
                    __M_writer(u'\n                        </div>\n')
                    # SOURCE LINE 66
                elif isinstance(smarthomeitem, WStateEntity):
                    # SOURCE LINE 67
                    __M_writer(u'                        <div class="smarthomeitem_state">\n                            Last written value: ')
                    # SOURCE LINE 68
                    __M_writer(filters.html_escape(unicode(repr(smarthomeitem.lastwrite) )))
                    __M_writer(u'\n                        </div>\n')
                # SOURCE LINE 71
                __M_writer(u'                </div>\n')
            # SOURCE LINE 73
            __M_writer(u'        </div>\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_title_main(context):
    __M_caller = context.caller_stack._push_frame()
    try:
        __M_writer = context.writer()
        # SOURCE LINE 6
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
        # SOURCE LINE 11
        __M_writer(u'\n')
        # SOURCE LINE 13
        __M_writer(u'    ')
        __M_writer(unicode(displayuniverse()))
        __M_writer(u'\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


