# -*- encoding:ascii -*-
from mako import runtime, filters, cache
UNDEFINED = runtime.UNDEFINED
__M_dict_builtin = dict
__M_locals_builtin = locals
_magic_number = 8
_modified_time = 1354741175.855947
_enable_loop = True
_template_filename = '/Users/christopher/Documents/University Stuff/2012-Senior Project/smarthome/src/ladybug/website/mako_templates/base/response.mako'
_template_uri = '/base/response.mako'
_source_encoding = 'ascii'
_exports = ['title_main', u'mainbody_content']


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
        response = context.get('response', UNDEFINED)
        def mainbody_content():
            return render_mainbody_content(context.locals_(__M_locals))
        repr = context.get('repr', UNDEFINED)
        __M_writer = context.writer()
        # SOURCE LINE 1
        __M_writer(u'\n\n')
        # SOURCE LINE 3
        __M_writer(u'\n\n\n')
        # SOURCE LINE 8
        __M_writer(u'\n\n')
        if 'parent' not in context._data or not hasattr(context._data['parent'], 'mainbody_content'):
            context['self'].mainbody_content(**pageargs)
        

        # SOURCE LINE 13
        __M_writer(u'\n\n\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_title_main(context):
    __M_caller = context.caller_stack._push_frame()
    try:
        __M_writer = context.writer()
        # SOURCE LINE 6
        __M_writer(u'\n    Response\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_mainbody_content(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        response = context.get('response', UNDEFINED)
        def mainbody_content():
            return render_mainbody_content(context)
        repr = context.get('repr', UNDEFINED)
        __M_writer = context.writer()
        # SOURCE LINE 10
        __M_writer(u'\n    Server returned:\n    <pre>')
        # SOURCE LINE 12
        __M_writer(unicode(repr(response)))
        __M_writer(u'</pre>\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


