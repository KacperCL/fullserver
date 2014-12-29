<?php
define("AV_LOGIN", "user");	// prosze wpisac swoj login do serwisu
define("AV_PASSWD", "pass");	// prosze wpisac swoje haslo do serwisu
define("AV_RPC_URL", "http://www.justpay.pl/smscodes/rpc");

/*
 * CodeCheckWrapper.php
 *
 * Wrapper do xmlrpc, ktory powinien dzialac na roznych konfiguracjach
 *
 * Michal Ostapowicz <michal.ostapowicz@gisql.pl>
 */

if (!defined("xmlrpc_encode_request") || substr(PHP_VERSION, 0, 1) < 5) {
	require_once("xmlrpc.inc");
}

class JustpaySMS {
	function validate($command, $la, $code) {
		if (!defined("xmlrpc_encode_request") || substr(PHP_VERSION, 0, 1) < 5) {
			return $this->dummyValidate($command, $la, $code);
		} else {
			return $this->normalValidate($command, $la, $code);
		}
	}

	function normalValidate($command, $la, $code) {
		$request = xmlrpc_encode_request("validate",
				array($command, $la, AV_LOGIN, AV_PASSWD, $code));
		$context = stream_context_create(array(
					'http' => array(
						'method' => "POST",
						'header' => "Content-Type: text/xml",
						'content' => $request
						)
					)
				);
		$file = file_get_contents(AV_RPC_URL, false, $context);
		$response = xmlrpc_decode($file);
		
		if (xmlrpc_is_fault($response)) {
			return false;
		}
		return $response['status'] == "OK";
	}

	function dummyValidate($command, $la, $code) {
		$host = preg_replace('/^http.*\/\/([^\/]*).*$/', '$1', AV_RPC_URL);
		$path = preg_replace('/^[^\.]*\.[^\/]*(\/.*)$/', '$1', AV_RPC_URL);

		$client = new xmlrpc_client($path, $host, 80);
		$message = new xmlrpcmsg("validate", array(
					new xmlrpcval($command, "string"),
					new xmlrpcval($la, "string"),
					new xmlrpcval(AV_LOGIN, "string"),
					new xmlrpcval(AV_PASSWD, "string"),
					new xmlrpcval($code, "string")
				)
			);
		$response = $client->send($message, 0);
//		var_dump ( $message );
//var_dump ( $response );
		if ($response->faultCode()) {
			return false;
		}

		$resp_val = $response->value();
		$status = $resp_val->structmem("status");
		return $status->scalarval() == "OK";
	}
}

?>
