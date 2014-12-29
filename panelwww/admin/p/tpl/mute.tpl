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
    <th>Wyciszony</th>
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
    <td class='zbanowany'>{$b.zbanowany}</td>
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
$(document).ready(function() {
    var banTable=$('table#bany').dataTable({
	"bPaginate": false,
	"oLanguage": { "sUrl": "/s/dataTables.polish.txt" }
	
	});
    $('table#bany td.klik').live('click', function(){
	    $("table#bany>tbody>tr.akcje").remove();
	    var bid=$(this).siblings("td.banid").html();
//	    alert(bid);
	    var nTr=$(this).parent("tr").after("<tr class='akcje'><td colspan='4'><button onclick='ban_remove("+bid+")'>Usun wyciszenie</button></td></tr>");
	    
//	    banTable.fnOpen( nTr, fnFormatDetails(banTable, nTr), 'details' );
	    
	});
} );

function fnFormatDetails ( banTable, nTr )
{
    var sOut="<div class='akcje'>Usuń wyciszenie</div>";
    
    return sOut;
}
			    

function ban_remove(banid){
    if (!window.confirm("Czy na pewno chcesz usunąć wyciszenie dla gracza "+$("table#bany>tbody>tr.bid"+banid+">td.zbanowany").html()+"?"))
	return;
    $.ajax({
	url: "/mute-ajax",
	data: { banid: banid, a2: "unban" },
	type: "POST",
	success: function(msg){
	    if (msg=='OK') {
		alert("Wyciszenie usuniete");
		$("table#bany>tbody>tr.bid"+banid+"+tr.akcje").remove();
		$("table#bany>tbody>tr.bid"+banid).remove();
	    } else alert("Usuwanie wyciszenia nie udalo sie sie."+msg);
	}
	})
}

{/literal}</script>


{include file="html.bottom.tpl"}