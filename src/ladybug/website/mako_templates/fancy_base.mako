<% templatecredit = True %>

<!DOCTYPE HTML>
<html>
    % if templatecredit:
    	<!-- This template was designed and developed by Chris Converse, Codify Design Studio -->
    % endif
	<head>
		<meta charset="UTF-8" />
        
        <title>${capture(local.title_full) | trim}</title>
        <meta name="description" content="${capture(local.page_description) | trim}" />
        ##<meta name="description" content="This site was created from a template originally designed and developed by Codify Design Studio. Find more free templates at http://www.adobe.com/devnet/author_bios/chris_converse.html" />
		
        <meta id="view" name="viewport" content="width=device-width, initial-scale=1.0" />
		<meta http-equiv="X-UA-Compatible" content="IE=9" />
        
        <link rel="stylesheet" type="text/css" href="${STATIC_URL}css/stylesheet.css" />
        ##<link rel="stylesheet" type="text/css" href="${STATIC_URL}css/stylesheet_extra.css" />

        ## support for IE < 9
		<!--[if lt IE 9]>
			<script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
			<link rel="stylesheet" type="text/css" href="${STATIC_URL}css/stylesheet_ie.css" />
		<![endif]-->
        
        <%block name="css_page"/>
        <%block name="javascript_page"/>
        
        <%block name="tweak_css_javascript"/>
	</head>
	<body>
        <%block name="body_contents">
    		<div class="page">
    			<header>
                    <%block name="header_contents">
                        <a class="logo"></a>
                    </%block>
                </header>
    			<div class="page_content">
                    <%block name="page_content_columns"/>
    				<nav>
                        <%block name="navigation_contents">
        					<a href="${url('website.views.index')}">Main</a>
                            <a href="${url('website.views.view')}">View</a>
        					<a href="${url('website.views.view_all')}">View All</a>
                        </%block>
    				</nav>
    			</div>
    			<footer>
                    Copyright &copy; 2012 Smart Home Team (L Balling, D. Bosnjak, C. Johnson, T. Rogers)
                </footer>
    		</div>
        </%block>
	</body>
</html>

<%def name="page_description()">
    This page is part of the website of the 2012 Smart Home senior project team at the University of Utah.
</%def>

<%!
    def precedewithspaceifnotempty(text):
        return u' ' + text.strip() if text and not text.isspace() else u''
%>

<%def name="title_full()">
    Smart Home - ${capture(self.title_main) | trim}
</%def>

<%def name="title_main()">
    <% raise AssertionError("page title must be specified") %>
</%def>

<%def name="django_template(template_text=None)">
    <%
        from django.template import Template, RequestContext

        if template_text == None:
            template_text = capture(caller.body)
        t = Template(template_text)
        context.write(t.render(RequestContext(request)))
    %>
</%def>
