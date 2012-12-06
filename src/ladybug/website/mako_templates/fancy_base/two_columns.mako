<%inherit file="/fancybase.mako"/>

<%block name="page_content_columns">
	<div class="page_content_container_left">
		<div class="content">
            <%block name="main_body_content"/>
		</div>
	</div>
    <%block name="sidebar">
		<div class="page_content_container_right">
			<div class="content_sidebar">
                <%block name="sidebar_contents"/>
				<div class="clear_both"></div>
			</div>
		</div>
    </%block>
	<div class="clear_both"></div>
</%block>
