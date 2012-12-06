<%inherit file="/base.mako"/>

<%! from mirabilis.core import RStateEntity, RWStateEntity, WStateEntity %>


<%def name="title_main()">
    Response
</%def>

<%block name="mainbody_content">
    Server returned:
    
    <pre>${repr(response)}</pre>
    
    <a href="${url('website.views.view_all')}">Return to View All</a>
</%block>


