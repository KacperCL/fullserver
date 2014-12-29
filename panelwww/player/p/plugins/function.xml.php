<?php

// function defination to convert array to xml
function array_to_xml($player_info, &$xml_player_info) {
	foreach($player_info as $key => $value) {
		if(is_array($value)) {
			if(!is_numeric($key)){
				$subnode = $xml_player_info->addChild("$key");
				array_to_xml($value, $subnode);
			}
			else{
				array_to_xml($value, $xml_player_info);
			}
		}
		else {
			$xml_player_info->addChild("$key","$value");
		}
	}
}

function smarty_function_xml($params, &$smarty) {
// initializing or creating array
		$player_info = $params[toXml];

		// creating object of SimpleXMLElement
		$xml_player_info = new SimpleXMLElement("<?xml version=\"1.0\"?><player_info></player_info>");

		// function call to convert array to xml
		array_to_xml($player_info,$xml_player_info);

		//print
		return $xml_player_info->asXML();
}

?>