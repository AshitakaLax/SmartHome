<% templatecredit = True %>

<!DOCTYPE HTML>
<html>
	<!-- This template was designed and developed by Chris Converse, Codify Design Studio -->
	<head>
		<meta charset="UTF-8" />
        <title>${capture(local.title_full) | trim}</title>
    	##<meta name="description" content="This site was created from a template originally designed and developed by Codify Design Studio. Find more free templates at http://www.adobe.com/devnet/author_bios/chris_converse.html" />
		<meta id="view" name="viewport" content="width=device-width, initial-scale=1.0" />
		<meta http-equiv="X-UA-Compatible" content="IE=9" />
        <%block name="css_links_global">
            <link rel="stylesheet" type="text/css" href="${STATIC_URL}css/stylesheet.css" />
        </%block>
        <%block name="css_links_page"/>
        <%block name="css_page_inline"/>
        <%block name="javascript_global"/>
        <%block name="javascript_page"/>
        <%block name="tweak_css_javascript">
    		<!--[if lt IE 9]>
    			<script src="http://html5shiv.googlecode.com/svn/trunk/html5.js"></script>
    			<link rel="stylesheet" type="text/css" href="${STATIC_URL}css/stylesheet_ie.css" />
    		<![endif]-->
        </%block>
	</head>
	<body>
        <%block name="body_contents">
    		<div class="page">
    			<header>
                    <%block name="header">
                        <a class="logo" href="#"></a>
                    </%block>
                </header>
    			<div class="page_content">
    				<div class="page_content_container_left">
    					<div class="content">
                            <%block name="main_body_content">
        						<h1>Home Status</h1>
        						<p>Temperature of room1,Temp2,</p>
        						<p>outlet power consumption</p>
        						<p>Damper Stats(which dampers are open or closed)</p>
        						<p>Room occup status</p>
        						<p>HVAC status</p>
        						<h1>Home Control</h1>
        						<p>//can change order</p>
        						<p>Button to Temperatures</p>
        						<p>Button for Outlets</p>
        						<p>Button for Dampers</p>
        						<p>Button for HVAC</p>
                                <p>&nbsp;</p>
                            </%block>
    					</div>
    				</div>
    				<div class="page_content_container_right">
    					<div class="content_sidebar">
                            <%block name="sidebar_content">
        						<h3>Outside info</h3>
        						<p>Integer vel augue. Phas ellus nul la purus inte rdum enatis fames ac turpis egestas.</p>
        						<figure>
        							<div class="figure_photo"></div>
        							<figcaption>Duis a eros lit ora tor quent per conu bia nos tra per.</figcaption>
        							<div class="clear_both"></div>
        						</figure>
        						<p>Pellent <a href="#">morbi tris</a> esque habitant senectus et netus et malesuada.</p>
        						<p>Integer vel augue. Phas ellus nul la purus inte rdum enatis.</p>
                            </%block>
    						<div class="clear_both"></div>
    					</div>
    				</div>
    				<div class="clear_both"></div>
    				<nav>
                        <%block name="navigation_content">
        					<a href="${url('website.views.index')}">Main Page</a>
        					<a href="${url('website.views.view_all')}">Debug Page</a>
                        </%block>
    				</nav>
    			</div>
    			<footer>
                    <%block name="footer_content">
                        Copyright &copy; 2012 Smart Home Team (L Balling, D. Bosnjak, C. Johnson, T. Rogers)
                    </%block>
                </footer>
    		</div>
        </%block>
	</body>
</html>

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
