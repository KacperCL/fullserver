{* Smarty *}

{include file="html.top.tpl"}

<form method='POST'>
<fieldset class='crit'><legend>Kryteria</legend>

<p>
    {if $LVL==1 OR $LVL>=4}<label><input type='checkbox' {if $k_level[1]}checked='checked'{/if} name='k_level[1]' /> moderatorzy</label>{/if}
    {if $LVL>=3}<label><input type='checkbox' {if $k_level[3]}checked='checked'{/if} name='k_level[3]' /> admini</label>{/if}
    {if $LVL>=4}<label><input type='checkbox' {if $k_level[4]}checked='checked'{/if} name='k_level[4]' /> rconi</label>{/if}
</p>
<p>
	<label><input type='checkbox' {if $k_view_tyg3}checked='checked'{/if} name='k_view_tyg3' /> widok 3+2+1</label>
	<label><input type='checkbox' {if $k_view_tyg4}checked='checked'{/if} name='k_view_tyg4' /> widok 2+1+b</label>
</p>
    <input type='submit' value='&raquo;' />
</fieldset>
</form>

{if $k_view_tyg4}

{foreach from=$aktywnosc_tygodniowa item=t key=tydzien}
<center><h3>Tydzień {$tydzien-2}+{$tydzien-1}+{$tydzien} (2+1+b)</h3></center><hr/>
<table class='full' id='aa'>
<thead>
  <tr>
	<th>ID</th>
	<th>Nick</th>
	<th align='right'>Norma</th>
	<th align='right'>Pozostało minut</th>
<th align='right'>Przegranych minut</th>
  </tr>

</thead>
<tbody>
{foreach from=$t item=w key=osoba}

  <tr class="level{$w.level}">
	<td align='right' width='7%'>{$w.id|intval}</td>
	<td width='33%'>{$osoba|htmlspecialchars}</td>
	<td align='right' width='33%'><div class='entry_progress'><div class='entry_bar{if $w.pozostalo<=0} zaliczone{/if}' style='width:{$w.norma|intval}%'>&nbsp;</div><div class="progress_text"><center>{$w.norma|intval}%</center></div></div></td>
	<td align='right'>{$w.pozostalo|intval}</td>
	<td align='right'>{$w.minutabs|intval}</td>	
  </tr>

{/foreach}
</tbody>
</table>
<br/>
{/foreach}

{else if $k_view_tyg3}

{foreach from=$aktywnosc_tygodniowa item=t key=tydzien}
<center><h3>Tydzień {$tydzien-3}+{$tydzien-2}+{$tydzien-1} (3+2+1)</h3></center><hr/>
<table class='full' id='aa'>
<thead>
  <tr>
	<th>ID</th>
	<th>Nick</th>
	<th align='right'>Norma</th>
	<th align='right'>Pozostało minut</th>
<th align='right'>Przegranych minut</th>
  </tr>

</thead>
<tbody>
{foreach from=$t item=w key=osoba}

  <tr class="level{$w.level}">
	<td align='right' width='7%'>{$w.id|intval}</td>
	<td width='33%'>{$osoba|htmlspecialchars}</td>
	<td align='right' width='33%'><div class='entry_progress'><div class='entry_bar{if $w.pozostalo<=0} zaliczone{/if}' style='width:{$w.norma|intval}%'>&nbsp;</div><div class="progress_text"><center>{$w.norma|intval}%</center></div></div></td>
	<td align='right'>{$w.pozostalo|intval}</td>
	<td align='right'>{$w.minutabs|intval}</td>	
  </tr>

{/foreach}
</tbody>
</table>
<br/>
{/foreach}

{else}

{foreach from=$aktywnosc_tygodniowa item=t key=tydzien}
<center><h3>Tydzień {$tydzien}</h3></center><hr/>
<table class='full' id='aa'>
<thead>
  <tr>
	<th>ID</th>
	<th>Nick</th>
	<th align='right'>Norma</th>
	<th align='right'>Pozostało minut</th>
<th align='right'>Przegranych minut</th>
  </tr>

</thead>
<tbody>
{foreach from=$t item=w key=osoba}

  <tr class="level{$w.level}">
	<td align='right' width='7%'>{$w.id|intval}</td>
	<td width='33%'>{$osoba|htmlspecialchars}</td>
	<td align='right' width='33%'><div class='entry_progress'><div class='entry_bar{if $w.pozostalo<=0} zaliczone{/if}' style='width:{$w.norma|intval}%'>&nbsp;</div><div class="progress_text"><center>{$w.norma|intval}%</center></div></div></td>
	<td align='right'>{$w.pozostalo|intval}</td>
	<td align='right'>{$w.minutabs|intval}</td>	
  </tr>

{/foreach}
</tbody>
</table>
<br/>
{/foreach}

{/if}

<script>{literal}

var k_abs=$("input[name=k_abs]:checked").length;

$(document).ready(function() {
    $('table#aa').dataTable({
	"bPaginate": false,
	"oLanguage": { "sUrl": "/s/dataTables.polish.txt" },
	"aoColumnDefs": [
	    {	"bSearchable": false, 
		"bUseRendered": false,
		"fnRender": function ( oObj ) {
			var m=oObj.aData[oObj.iDataColumn];
			if (m==0) return "-";
			if(!k_abs) {
			    var podstawa=((oObj.iDataColumn==6?3:(oObj.iDataColumn==7?3:1))*420);
			    if (m>=podstawa) return "0m";
			    m=podstawa-m;
			    return m+'m';
			}
			return Math.floor(m/60)+'h'+(m%60)+'m';
			
		 },
	        "aTargets": [ "minuty" ] }
	    
	]
	
	});
} );

{/literal}</script>

{include file="html.bottom.tpl"}
