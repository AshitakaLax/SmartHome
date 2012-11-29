<%inherit file="/base.mako"/>

<%! from mirabilis.core import RStateEntity, RWStateEntity, WStateEntity %>


<%def name="title_main()">
    universal view
</%def>


<%block name="mainbody_content">
    ##${universe | h}
    ${displayuniverse()}
</%block>


<%block name="credits">
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
    % for path in sorted(itemsbypath.keys()):
        <hr />
        <div class="pathanditems">
            <div class="pathname">
                ${path | h}
            </div>
            % for smarthomeitem in itemsbypath[path]:
                <div class="smarthomeitem">
                    Global ID: ${smarthomeitem.global_id}
                    <div class="smarthomeitem_body">
                        ${repr(smarthomeitem) | h}
                    </div>
                    % if isinstance(smarthomeitem, (RStateEntity, RWStateEntity)):
                        <div class="smarthomeitem_state">
                            Current state: ${repr(smarthomeitem.read()) | h}
                        </div>
                    % elif isinstance(smarthomeitem, WStateEntity):
                        <div class="smarthomeitem_state">
                            Last written value: ${repr(smarthomeitem.lastwrite) | h}
                        </div>
                    % endif
                </div>
            % endfor
        </div>
    % endfor
</%def>