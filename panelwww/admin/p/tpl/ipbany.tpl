{include file="html.top.tpl"}
<form method='POST'>
<fieldset class='crit'>
    <legend>Kryteria</legend>
    
    <p><label><input type='checkbox' name='aktywne' {if $k_aktywne}checked='checked'{/if} /> tylko aktywne</label></p>
    <p><label><input type='checkbox' name='playerinfo' {if $k_playerinfo}checked='checked'{/if} /> dolacz szczegoly o graczu</label></p>
    <p><label>Zalozone w ciągu: <select name='tl'>
    	<option value=''>bez ograniczeń</option>
	<option {if $k_tl=='24h'}selected='selected'{/if} value='24h'>ostatnich 24h</option>
	<option {if $k_tl=='48h'}selected='selected'{/if} value='48h'>ostatnich 48h</option>
	<option {if $k_tl=='4d'}selected='selected'{/if} value='4d'>ostatnich 4 dni</option>
	<option {if $k_tl=='1w'}selected='selected'{/if} value='1w'>ostatniego tygodnia</option>
	
	<option {if $k_tl=='1m'}selected='selected'{/if} value='1m'>ostatniego miesiąca</option>
    </select></label></p>
    <input type='submit' value='&raquo;' />
</fieldset>
</form>

<table class='full' id='bany'>
<thead>
<tr>
    <th>ID</th>
    {if $LVL>=4}<th>Akcja</th>{/if}
    <th>Zbanowany</th>
    <th>IP</th>
    {if $k_playerinfo}
	<th>Respekt</th>
	<th>Skill</th>
	<th>Czas gry</th>
    {/if}
    <th>Powod</th>
    <th>Przez</th>
    <th>Od</th>
    <th>Do</th>
    <th>Pozostało</th>
</tr>
</thead>
<tbody>
{foreach from=$bany item=b}
<tr class='bid{$b.id} {if $b.aktywny!=1}nieaktywny{/if}'>
    <td class='banid'>{$b.id}</td>
    {if $LVL>=4}<td class='klik{if $b.aktywny!=1}nieaktywny{/if}'>&nbsp;</td>{/if}
    <td class='zbanowany'{if $b.zbanowany} style='font-weight: bold;'{/if}>{$b.zbanowany|default:$b.target_nick}</td>
    <td class='ip'>{$b.ip}</td>
    {if $k_playerinfo}
	<td>{$b.respect}</td>
	<td>{$b.skill}</td>
	<td>{$b.minut}m</td>
    {/if}
    <td>{$b.reason}</td>
    <td>{$b.przez}</td>
    <td>{$b.date_created|date_format:"%d.%m.%Y %H:%M"}</td>
    <td>{$b.date_end|date_format:"%d.%m.%Y %H:%M"}</td>
    <td>{if $b.pozostaloh>0}{$b.pozostaloh/24|floor}d {$b.pozostaloh%24}h{else}-{/if}</td>
    

</tr>
{/foreach}
</tbody>
</table>
<script>{literal}
var a1,a2,a3,a4;
$(document).ready(function() {
    var banTable=$('table#bany').dataTable({
	"bPaginate": false,
	"oLanguage": { "sUrl": "/s/dataTables.polish.txt" }
	
	});
    $('table#bany td.klik').live('click', function(){
	    $("table#bany>tbody>tr.akcje").remove();
	    var bid=$(this).siblings("td.banid").html();
//	    alert(bid);
	    var nTr=$(this).parent("tr").after("<tr class='akcje'><td colspan='4'><button onclick='ban_remove("+bid+")'>Usun bana</button></td></tr>");
	    
//	    banTable.fnOpen( nTr, fnFormatDetails(banTable, nTr), 'details' );
	    
	});
	$("table#bany td.ip").live('click', function(){
	    $("#ipinfo>.zmienny").html("...");
	    $("#ipinfo>.xbl").html("...");	    
	    $("#ipinfo>.host").html("...");
	    $("#ipinfo>.hist").html("...");	    
	    $("#ipinfo>.ip").html($(this).html());   
	    if (a1)  a1.abort(); if (a2) a2.abort(); if (a3) a3.abort();
	     if (a4) a4.abort();
	    a1=$.ajax({url: "/ipbany-ajax", data: { a2: "rbl1", ip: $(this).html()}, type: "POST", success: function(data){	$("#ipinfo>.zmienny").html(data);    	}});
	    a3=$.ajax({url: "/ipbany-ajax", data: { a2: "rbl2", ip: $(this).html()}, type: "POST", success: function(data){	$("#ipinfo>.xbl").html(data);    	}});	    
	    a2=$.ajax({url: "/ipbany-ajax", data: { a2: "host", ip: $(this).html()}, type: "POST", success: function(data){	$("#ipinfo>.host").html(data);    	}});
	    a4=$.ajax({url: "/ipbany-ajax", data: { a2: "hist", ip: $(this).html()}, type: "POST", success: function(data){	$("#ipinfo>.hist").html(data);    	}});	    
	});
} );

function fnFormatDetails ( banTable, nTr )
{
    var sOut="<div class='akcje'>Usuń ban</div>";
    
    return sOut;
}
			    

function ban_remove(banid){
    if (!window.confirm("Czy na pewno chcesz usunąć bana na IP: "+$("table#bany>tbody>tr.bid"+banid+">td.ip").html()+"?"))
	return;
//	    return false;
    $.ajax({
	url: "/ipbany-ajax",
	data: { banid: banid, a2: "unban" },
	type: "POST",
	success: function(msg){
	    if (msg=='OK') {
		alert("Ban usuniety");
		$("table#bany>tbody>tr.bid"+banid+"+tr.akcje").remove();
		$("table#bany>tbody>tr.bid"+banid).remove();
	    } else alert("Usuwanie bana nie udalo sie sie."+msg);
	}
	})
}

{/literal}</script>
<div id='ipinfo'><span class='ip'></span>
<span class='host'></span>
<span class='zmienny'></span>
<span class='xbl'></span>
<span class='hist'></span></div>

{include file="html.bottom.tpl"}