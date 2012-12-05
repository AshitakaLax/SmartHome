# -*- encoding:ascii -*-
from mako import runtime, filters, cache
UNDEFINED = runtime.UNDEFINED
__M_dict_builtin = dict
__M_locals_builtin = locals
_magic_number = 8
_modified_time = 1354741093.882321
_enable_loop = True
_template_filename = u'/Users/christopher/Documents/University Stuff/2012-Senior Project/smarthome/src/ladybug/website/mako_templates/base.mako'
_template_uri = u'/base.mako'
_source_encoding = 'ascii'
_exports = [u'css_page_inline', u'footer_content', u'footer', 'title_full', u'mainbody_content', 'django_template', u'credits', 'title_main', u'css_links_page', u'header_javascript', u'css_links_global', u'document', 'bodyattrs']


# SOURCE LINE 48

def precedewithspaceifnotempty(text):
    return (u' ' + text.strip()) if (text and not text.isspace()) else u''


def render_body(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        __M_locals = __M_dict_builtin(pageargs=pageargs)
        capture = context.get('capture', UNDEFINED)
        def css_page_inline():
            return render_css_page_inline(context.locals_(__M_locals))
        def footer_content():
            return render_footer_content(context.locals_(__M_locals))
        def footer():
            return render_footer(context.locals_(__M_locals))
        self = context.get('self', UNDEFINED)
        def mainbody_content():
            return render_mainbody_content(context.locals_(__M_locals))
        def credits():
            return render_credits(context.locals_(__M_locals))
        def css_links_page():
            return render_css_links_page(context.locals_(__M_locals))
        def header_javascript():
            return render_header_javascript(context.locals_(__M_locals))
        def css_links_global():
            return render_css_links_global(context.locals_(__M_locals))
        def document():
            return render_document(context.locals_(__M_locals))
        local = context.get('local', UNDEFINED)
        __M_writer = context.writer()
        if 'parent' not in context._data or not hasattr(context._data['parent'], 'document'):
            context['self'].document(**pageargs)
        

        # SOURCE LINE 46
        __M_writer(u'\n\n')
        # SOURCE LINE 51
        __M_writer(u'\n\n')
        # SOURCE LINE 55
        __M_writer(u'\n\n')
        # SOURCE LINE 59
        __M_writer(u'\n\n')
        # SOURCE LINE 61
        __M_writer(u'\n\n')
        # SOURCE LINE 72
        __M_writer(u'\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_css_page_inline(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        def css_page_inline():
            return render_css_page_inline(context)
        __M_writer = context.writer()
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_footer_content(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        def credits():
            return render_credits(context)
        def footer_content():
            return render_footer_content(context)
        __M_writer = context.writer()
        # SOURCE LINE 23
        __M_writer(u'\n                            <div id="credits">\n                                ')
        if 'parent' not in context._data or not hasattr(context._data['parent'], 'credits'):
            context['self'].credits(**pageargs)
        

        # SOURCE LINE 38
        __M_writer(u'\n                            </div>\n                        ')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_footer(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        def credits():
            return render_credits(context)
        def footer_content():
            return render_footer_content(context)
        def footer():
            return render_footer(context)
        __M_writer = context.writer()
        # SOURCE LINE 21
        __M_writer(u'\n                    <div id="footer">\n                        ')
        if 'parent' not in context._data or not hasattr(context._data['parent'], 'footer_content'):
            context['self'].footer_content(**pageargs)
        

        # SOURCE LINE 40
        __M_writer(u'\n                    </div>\n                ')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_title_full(context):
    __M_caller = context.caller_stack._push_frame()
    try:
        capture = context.get('capture', UNDEFINED)
        self = context.get('self', UNDEFINED)
        __M_writer = context.writer()
        # SOURCE LINE 53
        __M_writer(u'\n    Smart Home - ')
        # SOURCE LINE 54
        __M_writer(filters.trim(unicode(capture(self.title_main) )))
        __M_writer(u'\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_mainbody_content(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        def mainbody_content():
            return render_mainbody_content(context)
        __M_writer = context.writer()
        # SOURCE LINE 17
        __M_writer(u'\n                        The main body should go here.\n                    ')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_django_template(context,template_text=None):
    __M_caller = context.caller_stack._push_frame()
    try:
        capture = context.get('capture', UNDEFINED)
        caller = context.get('caller', UNDEFINED)
        request = context.get('request', UNDEFINED)
        __M_writer = context.writer()
        # SOURCE LINE 63
        __M_writer(u'\n    ')
        # SOURCE LINE 64

        from django.template import Template, RequestContext
        
        if template_text == None:
            template_text = capture(caller.body)
        t = Template(template_text)
        context.write(t.render(RequestContext(request)))
            
        
        # SOURCE LINE 71
        __M_writer(u'\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_credits(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        def credits():
            return render_credits(context)
        __M_writer = context.writer()
        # SOURCE LINE 25
        __M_writer(u'\n                                    <hr />\n                                    Smart Home Server credits\n                                    <br />\n                                    <br />\n                                    Coded by Christopher Johnson\n                                    <br />\n                                    Assisted by Levi Balling, Dario Bosnjak, and Todd Rogers\n                                    <br />\n                                    <br />\n                                    Implemented using Python programming language and libraries\n                                    <br />\n                                    Built on Django web framework and Mako templating engine\n                                ')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_title_main(context):
    __M_caller = context.caller_stack._push_frame()
    try:
        AssertionError = context.get('AssertionError', UNDEFINED)
        __M_writer = context.writer()
        # SOURCE LINE 57
        __M_writer(u'\n    ')
        # SOURCE LINE 58
        raise AssertionError("page title must be specified") 
        
        __M_writer(u'\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_css_links_page(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        def css_links_page():
            return render_css_links_page(context)
        __M_writer = context.writer()
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_header_javascript(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        def header_javascript():
            return render_header_javascript(context)
        __M_writer = context.writer()
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_css_links_global(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        def css_links_global():
            return render_css_links_global(context)
        __M_writer = context.writer()
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_document(context,**pageargs):
    __M_caller = context.caller_stack._push_frame()
    try:
        capture = context.get('capture', UNDEFINED)
        def css_page_inline():
            return render_css_page_inline(context)
        def footer_content():
            return render_footer_content(context)
        def footer():
            return render_footer(context)
        self = context.get('self', UNDEFINED)
        def mainbody_content():
            return render_mainbody_content(context)
        def credits():
            return render_credits(context)
        def css_links_page():
            return render_css_links_page(context)
        def header_javascript():
            return render_header_javascript(context)
        def css_links_global():
            return render_css_links_global(context)
        def document():
            return render_document(context)
        local = context.get('local', UNDEFINED)
        __M_writer = context.writer()
        # SOURCE LINE 1
        __M_writer(u'\n    <!DOCTYPE html>\n    <html>\n        <head>\n            <meta charset="UTF-8">\n            <title>')
        # SOURCE LINE 6
        __M_writer(filters.trim(unicode(capture(local.title_full) )))
        __M_writer(u'</title>\n            ')
        if 'parent' not in context._data or not hasattr(context._data['parent'], 'css_links_global'):
            context['self'].css_links_global(**pageargs)
        

        # SOURCE LINE 7
        __M_writer(u'\n            ')
        if 'parent' not in context._data or not hasattr(context._data['parent'], 'css_links_page'):
            context['self'].css_links_page(**pageargs)
        

        # SOURCE LINE 8
        __M_writer(u'\n            ')
        if 'parent' not in context._data or not hasattr(context._data['parent'], 'css_page_inline'):
            context['self'].css_page_inline(**pageargs)
        

        # SOURCE LINE 9
        __M_writer(u'\n            ')
        if 'parent' not in context._data or not hasattr(context._data['parent'], 'header_javascript'):
            context['self'].header_javascript(**pageargs)
        

        # SOURCE LINE 10
        __M_writer(u'\n        </head>\n        <body')
        # SOURCE LINE 12
        __M_writer(precedewithspaceifnotempty(unicode(capture(self.bodyattrs) )))
        __M_writer(u'>\n            <div id="single-column">\n                <div id="header">\n                </div>\n                <div id="mainbody">\n                    ')
        if 'parent' not in context._data or not hasattr(context._data['parent'], 'mainbody_content'):
            context['self'].mainbody_content(**pageargs)
        

        # SOURCE LINE 19
        __M_writer(u'\n                </div>\n                ')
        if 'parent' not in context._data or not hasattr(context._data['parent'], 'footer'):
            context['self'].footer(**pageargs)
        

        # SOURCE LINE 42
        __M_writer(u'\n            </div>\n        </body>\n    </html>\n')
        return ''
    finally:
        context.caller_stack._pop_frame()


def render_bodyattrs(context):
    __M_caller = context.caller_stack._push_frame()
    try:
        __M_writer = context.writer()
        return ''
    finally:
        context.caller_stack._pop_frame()


