<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="pl" >
<head>
    <meta http-equiv="Content-type" content="text/html; charset=UTF-8" />
    <title>System administracji i zarzadzania offline - FullServer.eu</title>
    <script src='s/jquery.js' type='text/javascript'></script>
    <script src='s/jquery.dataTables.min.js' type='text/javascript'></script>
    <link rel='stylesheet' href='s/style.css' type='text/css'>
	<link rel="stylesheet" href="s/ui/1.10.3/themes/smoothness/jquery-ui.css" />

	<script src="s/ui/1.10.3/jquery-ui.js"></script>
	<script>
		$(function() {
			$( "#datepicker" ).datepicker();
		});
	</script>
    <style type="text/css" title="currentStyle">
        @import "s/demo_page.css";
	@import "s/demo_table.css";
    </style>
										    
</head>
<body>
<ul id='nav'>
{if $LVL>=4}    <li><a href='/aktywnosc'>Aktywnosc (widok standartowy)</a></li>
    <li><a href='/aktywnosc-adv'>Aktywnosc (widok zaawansowany)</a></li>
	<li><a href='/rankb'>Ranking banow</a></li>
    <li><a href='/bany'>Bany na konta</a></li>
    <li><a href='/ipbany'>Bany na IP</a></li>
	<li><a href='/mute'>Wyciszenia</a></li>
	<li><a href='/nowyban'>Dodaj nowy ban</a></li>
	<li><a href='/szukaj'>Przeszukaj baze graczy</a></li>
	<li><a href='/wyloguj'>Wyloguj</a></li>{/if}
	{if $LVL==3}
	<li><a href='/aktywnosc'>Aktywnosc</a></li>
    <li><a href='/bany'>Bany na konta (tylko odczyt)</a></li>
    <li><a href='/ipbany'>Bany na IP (tylko odczyt)</a></li>
	<li><a href='/mute'>Wyciszenia (tylko odczyt)</a></li>
	<li><a href='/nowyban'>Dodaj nowy ban</a></li>
    <li><a href='/wyloguj'>Wyloguj</a></li>{/if}
	{if $LVL==2}
	<li><a href='/aktywnosc'>Aktywnosc</a></li>
    <li><a href='/bany'>Bany na konta (tylko odczyt)</a></li>
    <li><a href='/ipbany'>Bany na IP (tylko odczyt)</a></li>
	<li><a href='/mute'>Wyciszenia (tylko odczyt)</a></li>
    <li><a href='/wyloguj'>Wyloguj</a></li>{/if}
	{if $LVL==1}
	<li><a href='/aktywnosc'>Aktywnosc</a></li>
    <li><a href='/wyloguj'>Wyloguj</a></li>{/if}
</ul>
