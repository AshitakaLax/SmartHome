<%inherit file="/fancy_base/two_columns.mako"/>

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
    View
</%def>

<%block name="main_body_content">
    <% 
        lab = universe["MEB"][3][3143]
        epic_cube = lab["Epic Cube Device"] 
        pstripandsensors = lab["Power Strip and Sensors"]
    %>
    <h1>Home Control</h1>
    
    <h2>Temperature</h2>
    <div>Current temperature: ${pstripandsensors.temperature.read() | h}</div>
    
    <h2>Outlets</h2>
    <div style="margin-left: 25px">
        <h3>On/off Status</h3>
        <div>Outlet 1: ${outletchangeform(pstripandsensors.outlet1onoff)}</div>
        <div>Outlet 2: ${outletchangeform(pstripandsensors.outlet2onoff)}</div>
        <div>Outlet 3: ${outletchangeform(pstripandsensors.outlet3onoff)}</div>
        <div>Outlet 4: ${outletchangeform(pstripandsensors.outlet4onoff)}</div>
        <h3>Power consumption</h3>
        <div>Outlet 1: ${pstripandsensors.outlet1power.read() | h}</div>
        <div>Outlet 2: ${pstripandsensors.outlet2power.read() | h}</div>
        <div>Outlet 3: ${pstripandsensors.outlet3power.read() | h}</div>
        <div>Outlet 4: ${pstripandsensors.outlet4power.read() | h}</div>
    </div>
    
    <h2>Room occupied status</h2>
    % if "UNINITIALIZED" in repr(pstripandsensors.lightstatus.read()):
        <div>Room occupancy is in an indeterminate state.</div>
    % elif pstripandsensors.lightstatus.read() in (True, "True", "1"):
        <div>Room is occupied and automatic light is therefore ON.</div>
    % else:
        <div>Room is vacant and automatic light is therefore OFF.</div>
    % endif
    
    <%doc>
        <h2>Temperatures</h2>
        <div>Temperature 1: ${epic_cube.tempsensors[0].read() | h}</div>
        <div>Temperature 2: ${epic_cube.tempsensors[1].read() | h}</div>
    </%doc>
    <%doc>
        <h2>Damper Stats(which dampers are open or closed)</h2>
        % for number, damper in sorted(epic_cube.dampers.items()):
            <div>Damper ${number}: ${damper.read() | h}</div>
        % endfor
    </%doc>
    
    <h2>Heating/Ventilation/Air Conditioning</h2>
    <div>HVAC status: ${epic_cube.hvacstatus.read()}</div>
    <div>${hvac_change_form(epic_cube.hvac_command, 1, "thermostat controlled")}</div>
    <div>${hvac_change_form(epic_cube.hvac_command, 2, "server controlled, blower on")}</div>
    <div>${hvac_change_form(epic_cube.hvac_command, 3, "server controlled, blower and heating on")}</div>
    <div>${hvac_change_form(epic_cube.hvac_command, 4, "server controlled, blower and AC on")}</div>
    <div>${hvac_change_form(epic_cube.hvac_command, 5, "thermostat controlled")}</div>
    
    <%doc>
        <h2>Dampers</h2>
        % for number, damper in sorted(epic_cube.dampers.items()):
            <div>Damper ${number}: ${damperchangeform(damper)}</div>
        % endfor
    </%doc>
    
    <h2>Garage Door</h2>
    <form action="${url('website.views.change')}" method="post">
        ${csrf_protect()}
        <input type="hidden" name="global_id" value="${epic_cube.garage.global_id}" />
        <input type="hidden" name="writedata" value="" />
        <input type="submit" value="Toggle Garage Door" />
    </form>
    
    <h2>Sprinklers</h2>
    <div style="margin-left: 25px">
        <h3>Sprinklers A control</h3>
        <div>
            % for x in range(6):
                <div>
                    Sprinkler ${x}: 
                    ${showsprinklerchangeform(epic_cube.sprinklers[x], "ON")}
                    ${showsprinklerchangeform(epic_cube.sprinklers[x], "OFF")}
                </div>
            % endfor
        </div>
        <h3>Autonomous sprinkler B monitoring</h3>
        Sprinklers are ${pstripandsensors.sprinklers.read()}
    </div>
    <h2>
    
    <h2>Fan Control</h2>
    ##% for x in [0, 2, 3, 4]:
    % for x in [0]:
        <div>
            Fan ${x}: ${epic_cube.fans[x].read()}.
            Change to: 
            % for newstate in ["OFF"] + list(map(repr, range(1, 10 + 1))):
                ${showfanchangeform(epic_cube.fans[x], newstate)}
            % endfor
        </div>
    % endfor
</%block>

<%def name="showfanchangeform(fan, newstate)">
    <form style="display: inline;" action="${url('website.views.change')}" method="post">
        ${csrf_protect()}
        <input type="hidden" name="global_id" value="${fan.global_id}" />
        <input type="hidden" name="writedata" value="${newstate}" />
        <input type="submit" value="${newstate}" />
    </form>
</%def>

<%def name="showsprinklerchangeform(sprinkler, onoroff)">
    <form style="display: inline;" action="${url('website.views.change')}" method="post">
        ${csrf_protect()}
        <input type="hidden" name="global_id" value="${sprinkler.global_id}" />
        <input type="hidden" name="writedata" value="${onoroff}" />
        <input type="submit" value="Turn ${onoroff}" />
    </form>
</%def>

<%! 
    def invnumbool(s):
        """
        invert numeric boolean.  take string, return string
        """
        i = int(s)
        assert i in (0, 1)
        b = bool(i)
        inv = not b
        inv_i = int(inv)
        assert inv_i in (0, 1)
        return repr(inv_v)
%>

<%def name="showchangeform(smarthomeitem, setting=None)">
    <% 
        assert UNINITIALIZED != UNDEFINED
        assert NEVER_WRITTEN != UNDEFINED
                
        if not isinstance(smarthomeitem, (RWStateEntity, WStateEntity)):
            readonly = 'disabled="disabled"'
        else:
            readonly = ""
                
        if isinstance(smarthomeitem, (RStateEntity, RWStateEntity)):
            ##prefix = "Current state:"
            display = smarthomeitem.read()
        elif isinstance(smarthomeitem, WStateEntity):
            ##prefix = "Last written value:"
            display = smarthomeitem.lastwrite
        else:
            assert False  # not reached
                
        #print UNINITIALIZED, ":", id(UNINITIALIZED)
        #print display, ":", id(display)
        #print UNINITIALIZED == display
        if display == UNINITIALIZED or display == NEVER_WRITTEN:
            display = ""
        else:
            display = str(display)  # + repr(type(display))
            #display = ""
    %>
        
    <form style="display: inline;" action="${url('website.views.change')}" method="post">
        ${csrf_protect()}
        <input type="hidden" name="global_id" value="${smarthomeitem.global_id}" />
        % if setting is None:
            <input type="text" name="writedata" value="${display | h}" ${readonly} />
            % if not readonly:
                <input type="submit" value="Change" />
            % endif
        % else:
            <input type="hidden" name="writedata" value="${setting}" />
            % if not readonly:
                <input type="submit" value="Change to ${setting}">
            % endif
        % endif
    </form>
</%def>

<%def name="showchangeformforboolentity(smarthomeitem)">
    <% 
        try:
            result = invnumbool(smarthomeitem.read())
        except:
            success = False
        else:
            success = True
    %>
    % if success:
        ${showchangeform(smarthomeitem, invnumbool())}
    % else:
        ${showchangeform(smarthomeitem)}
    % endif
</%def>

<%def name="outletchangeform(smarthomeitem)">
    <%doc>
        <% 
            assert UNINITIALIZED != UNDEFINED
            assert NEVER_WRITTEN != UNDEFINED
                
            if not isinstance(smarthomeitem, (RWStateEntity, WStateEntity)):
                readonly = 'disabled="disabled"'
            else:
                readonly = ""
                
            if isinstance(smarthomeitem, (RStateEntity, RWStateEntity)):
                ##prefix = "Current state:"
                display = smarthomeitem.read()
            elif isinstance(smarthomeitem, WStateEntity):
                ##prefix = "Last written value:"
                display = smarthomeitem.lastwrite
            else:
                assert False  # not reached
                
            #print UNINITIALIZED, ":", id(UNINITIALIZED)
            #print display, ":", id(display)
            #print UNINITIALIZED == display
            if display == UNINITIALIZED or display == NEVER_WRITTEN:
                display = ""
            else:
                display = str(display)  # + repr(type(display))
                #display = ""
        %>
    </%doc>
    <%
        display = smarthomeitem.read()
        setting = "ON" if display == "OFF" else "OFF"
    %>
    <form style="display: inline;" action="${url('website.views.change')}" method="post">
        ${csrf_protect()}
        ${display}
        <input type="hidden" name="global_id" value="${smarthomeitem.global_id}" />
        <input type="hidden" name="writedata" value="${setting}" />
        <input type="submit" value="Change to ${setting}">
    </form>
</%def>

<%def name="damperchangeform(smarthomeitem)">
    <%doc>
        <% 
            assert UNINITIALIZED != UNDEFINED
            assert NEVER_WRITTEN != UNDEFINED
                
            if not isinstance(smarthomeitem, (RWStateEntity, WStateEntity)):
                readonly = 'disabled="disabled"'
            else:
                readonly = ""
                
            if isinstance(smarthomeitem, (RStateEntity, RWStateEntity)):
                ##prefix = "Current state:"
                display = smarthomeitem.read()
            elif isinstance(smarthomeitem, WStateEntity):
                ##prefix = "Last written value:"
                display = smarthomeitem.lastwrite
            else:
                assert False  # not reached
                
            #print UNINITIALIZED, ":", id(UNINITIALIZED)
            #print display, ":", id(display)
            #print UNINITIALIZED == display
            if display == UNINITIALIZED or display == NEVER_WRITTEN:
                display = ""
            else:
                display = str(display)  # + repr(type(display))
                #display = ""
        %>
    </%doc>
    <%
        display = smarthomeitem.read()
        setting = "OPEN" if display == "CLOSED" else "CLOSED"
    %>
    <form style="display: inline;" action="${url('website.views.change')}" method="post">
        ${csrf_protect()}
        ${display}
        <input type="hidden" name="global_id" value="${smarthomeitem.global_id}" />
        <input type="hidden" name="writedata" value="${setting}" />
        <input type="submit" value="Change to ${setting}">
    </form>
</%def>

<%def name="hvac_change_form(smarthomeitem, number, text)">
    <%doc>
        <% 
            assert UNINITIALIZED != UNDEFINED
            assert NEVER_WRITTEN != UNDEFINED
                
            if not isinstance(smarthomeitem, (RWStateEntity, WStateEntity)):
                readonly = 'disabled="disabled"'
            else:
                readonly = ""
                
            if isinstance(smarthomeitem, (RStateEntity, RWStateEntity)):
                ##prefix = "Current state:"
                display = smarthomeitem.read()
            elif isinstance(smarthomeitem, WStateEntity):
                ##prefix = "Last written value:"
                display = smarthomeitem.lastwrite
            else:
                assert False  # not reached
                
            #print UNINITIALIZED, ":", id(UNINITIALIZED)
            #print display, ":", id(display)
            #print UNINITIALIZED == display
            if display == UNINITIALIZED or display == NEVER_WRITTEN:
                display = ""
            else:
                display = str(display)  # + repr(type(display))
                #display = ""
        %>
    </%doc>
    <form style="display: inline;" action="${url('website.views.change')}" method="post">
        ${csrf_protect()}
        <input type="hidden" name="global_id" value="${smarthomeitem.global_id}" />
        <input type="hidden" name="writedata" value="${number}" />
        <input type="submit" value="Change to ${number} (${text})">
    </form>
</%def>