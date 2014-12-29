{* Smarty *}

{include file="_header.tpl"}

{literal}<style>
#table-2 {
	border: 1px solid #e3e3e3;
	background-color: #f2f2f2;
	border-radius: 6px;
	-webkit-border-radius: 6px;
	-moz-border-radius: 6px;
}
#table-2 td, #table-2 th {
	padding: 5px;
	color: #333;
}
#table-2 thead {
	font-family: "Lucida Sans Unicode", "Lucida Grande", sans-serif;
	padding: .2em 0 .2em .5em;
	color: #4B4B4B;
	background-color: #C8C8C8;
	background-image: -webkit-gradient(linear, left top, left bottom, from(#f2f2f2), to(#e3e3e3), color-stop(.6,#B3B3B3));
	background-image: -moz-linear-gradient(top, #D6D6D6, #B0B0B0, #B3B3B3 90%);
	border-bottom: solid 1px #999;
}
#table-2 th {
	font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif;
	font-size: 17px;
	line-height: 20px;
	font-style: normal;
	font-weight: normal;
	text-shadow: white 1px 1px 1px;
}
#table-2 td {
	line-height: 20px;
	font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif;
	font-size: 14px;
	border-bottom: 1px solid #fff;
	border-top: 1px solid #fff;
}
#table-2 td:hover {
	background-color: #fff;
}</style>{/literal}

{assign var=plik value=file('/home/fullserv/public_html/fs.i32.pl/changelog.txt')}
{assign var=tmpcount value=count($plik)}
<br/>
<div align="center">
<table id="table-2">
<thead>
<tr>
<th>Opis</th>
<th>Data</th>
<th>Typ</th>
</tr>
</thead>
<tbody>

{for $i=0 to $tmpcount-1}

{assign var=l value=explode(':', $plik[$i])}

<tr>
<td>
	{$l[0]}
</td>
<td>
	{$l[1]}
</td>
<td>
	{$l[2]}
</td>
</tr>


{/for}
</tr>
</tbody>
</table>
</div>
{include file="_footer.tpl"}