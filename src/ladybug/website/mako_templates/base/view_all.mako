<%inherit file="/base.mako"/>

<%!
    from mirabilis.core import (RStateEntity, 
                                RWStateEntity, 
                                WStateEntity,
                                StateEntity, 
                                UNINITIALIZED, 
                                NEVER_WRITTEN)
    assert UNINITIALIZED != UNDEFINED
    assert NEVER_WRITTEN != UNDEFINED
%>


<%def name="title_main()">
    universal view
</%def>

<%block name="mainbody_content">
    ##${universe | h}
    ${displayuniverse()}
</%block>


<%def name="displayitem(smarthomeitem)">
    ## Global ID: ${smarthomeitem.global_id}
    <td class="smarthomeitem_body">
        ${repr(smarthomeitem) | h}
    </td>
    <td class="smarthomeitem_state">
        % if isinstance(smarthomeitem, StateEntity):
            <% 
                assert UNINITIALIZED != UNDEFINED
                assert NEVER_WRITTEN != UNDEFINED
                
                if not isinstance(smarthomeitem, (RWStateEntity, WStateEntity)):
                    readonly = 'disabled="disabled"'
                else:
                    readonly = ""
                
                if isinstance(smarthomeitem, (RStateEntity, RWStateEntity)):
                    prefix = "Current state:"
                    display = smarthomeitem.read()
                elif isinstance(smarthomeitem, WStateEntity):
                    prefix = "Last written value:"
                    display = smarthomeitem.lastwrite
                else:
                    assert False  # not reached
                
                #print UNINITIALIZED, ":", id(UNINITIALIZED)
                #print display, ":", id(display)
                #print UNINITIALIZED == display
                if display in (UNINITIALIZED, NEVER_WRITTEN):
                    display = ""
                else:
                    display = repr(display) + repr(type(display))
                    #display = ""
            %>
        
            <form action="${url('website.views.change')}" method="post">
                ${prefix}
                ${csrf_protect()}
                <input type="hidden" name="global_id" value="${smarthomeitem.global_id}" />
                <input type="text" name="writedata" value="${display | h}" ${readonly} />
                <input type="submit" value="Change" />
            </form>
        % endif
    </td>
</%def>

<%def name="displayuniverse()">
    <p>
        Universe with 
        ${len(universe.devices)} devices 
        and ${len(universe.objectsdict)} items.
    </p>
    <% 
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
    %>
    <table border="1">
        % for path in sorted(itemsbypath.keys()):
            <% 
                assert len(itemsbypath[path])
                myiter = iter(itemsbypath[path])
            %>
            <tr>
                <td class="pathname" rowspan="${len(itemsbypath[path])}">
                    ${path | h}
                </td>
                ${displayitem(myiter.next())}
            </tr>
            % for smarthomeitem in myiter:
                <tr>
                    ${displayitem(smarthomeitem)}
                </tr>
            % endfor
        % endfor
    </table>
</%def>