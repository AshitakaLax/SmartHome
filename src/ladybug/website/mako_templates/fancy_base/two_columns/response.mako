<%inherit file="/fancy_base/two_columns.mako"/>

<%! from mirabilis.core import RStateEntity, RWStateEntity, WStateEntity %>


<%def name="title_main()">
    Response
</%def>

<%block name="main_body_content">
    Server returned:
    
    <pre>${repr(response)}</pre>
    
    <a href="${url('website.views.view')}">Return to Main</a>
</%block>


