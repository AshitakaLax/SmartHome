<%block name="document">
    <!DOCTYPE html>
    <html>
        <head>
            <meta charset="UTF-8">
            <title>${capture(local.title_full) | trim}</title>
            <%block name="css_links_global"/>
            <%block name="css_links_page"/>
            <%block name="css_page_inline"/>
            <%block name="header_javascript"/>
        </head>
        <body${capture(self.bodyattrs) | precedewithspaceifnotempty}>
            <div id="single-column">
                <div id="header">
                </div>
                <div id="mainbody">
                    <%block name="mainbody_content">
                        The main body should go here.
                    </%block>
                </div>
                <%block name="footer">
                    <div id="footer">
                        <%block name="footer_content">
                            <div id="credits">
                                <%block name="credits">
                                    <hr />
                                    Smart Home Server credits
                                    <br />
                                    <br />
                                    Coded by Christopher Johnson
                                    <br />
                                    Assisted by Levi Balling, Dario Bosnjak, and Todd Rogers
                                    <br />
                                    <br />
                                    Implemented using Python programming language and libraries
                                    <br />
                                    Built on Django web framework and Mako templating engine
                                </%block>
                            </div>
                        </%block>
                    </div>
                </%block>
            </div>
        </body>
    </html>
</%block>

<%!
    def precedewithspaceifnotempty(text):
        return (u' ' + text.strip()) if (text and not text.isspace()) else u''
%>

<%def name="title_full()">
    Smart Home - ${capture(self.title_main) | trim}
</%def>

<%def name="title_main()">
    <% raise AssertionError("page title must be specified") %>
</%def>

<%def name="bodyattrs()"/>

<%def name="django_template(template_text=None)">
    <%
        from django.template import Template, RequestContext

        if template_text == None:
            template_text = capture(caller.body)
        t = Template(template_text)
        context.write(t.render(RequestContext(request)))
    %>
</%def>
