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
    <h1>Home Status</h1>
    
    <h2>Temperature of room1,Temp2,</h2>
    <div>Temperature 1: ${epic_cube.tempsensors[0].read() | h}</div>
    <div>Temperature 2: ${epic_cube.tempsensors[1].read() | h}</div>
    
    <h2>outlet power consumption</h2>
    <div>Outlet 1: ${pstripandsensors.outlet1power.read() | h}</div>
    <div>Outlet 2: ${pstripandsensors.outlet2power.read() | h}</div>
    <div>Outlet 3: ${pstripandsensors.outlet3power.read() | h}</div>
    <div>Outlet 4: ${pstripandsensors.outlet4power.read() | h}</div>
    
    <h2>Damper Stats(which dampers are open or closed)</h2>
    % for number, damper in sorted(epic_cube.dampers.items()):
        <div>Damper ${number}: ${damper.read() | h}</div>
    % endfor
    
    <h2>Room occupied status</h2>
    % if "UNINITIALIZED" in repr(pstripandsensors.lightstatus.read()):
        <div>Room occupancy is in an indeterminate state.</div>
    % elif pstripandsensors.lightstatus.read() in (True, "True", "1"):
        <div>Room is occupied and automatic light is therefore ON.</div>
    % else:
        <div>Room is vacant and automatic light is therefore OFF.</div>
    % endif
    <h2>HVAC status</h2>
    <div></div>
    
    <h1>Home Control</h1>
    ##<p>//can change order</p>
    ##<p>Button to Temperatures</p>   -- can't change thermometers
    <h2>Button for Outlets</h2>
    <div>Outlet 1: ${showchangeformforboolentity(pstripandsensors.outlet1onoff)}</div>
    <div>Outlet 2: ${showchangeformforboolentity(pstripandsensors.outlet2onoff)}</div>
    <div>Outlet 3: ${showchangeformforboolentity(pstripandsensors.outlet3onoff)}</div>
    <div>Outlet 4: ${showchangeformforboolentity(pstripandsensors.outlet4onoff)}</div>
    <p>Button for Dampers</p>
    <p>Button for HVAC</p>
    <p>&nbsp;</p>
</%block>

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