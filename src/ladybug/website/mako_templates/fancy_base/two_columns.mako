<%inherit file="/fancy_base.mako"/>

<%block name="page_content_columns">
	<div class="page_content_container_left">
		<div class="content">
            <%block name="main_body_content"/>
		</div>
	</div>
    <%block name="sidebar">
		<div class="page_content_container_right">
			<div class="content_sidebar">
                <%block name="sidebar_contents">
                    <h1>SMART HOME</h1>
                    <p>created by</p>
                    <p>
                        Christopher Johnson
                        <br />
                        Dario Bosnjak
                        <br />
                        Levi Balling
                        <br />
                        Todd Rogers
                    </p>
                </%block>
				<div class="clear_both"></div>
			</div>
		</div>
    </%block>
	<div class="clear_both"></div>
</%block>
