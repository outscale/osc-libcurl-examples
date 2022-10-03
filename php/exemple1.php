<?php

//
// REQUIRE PHP 8.2 OR SUPERRIOR
// This message been writed in 2022. Most likelly you need to recompille it.
// it's like ./buildroor; ./configure --with-curl; make -jNUMBER_OF_CORE_FLEX; ./sapi/cli/php PATH/TO/THIS/FILE
//

function call($c, $call, $arg) {
    curl_setopt($c, CURLOPT_URL, "https://api.eu-west-2.outscale.com/api/v1/".$call);
    curl_setopt($c, CURLOPT_POSTFIELDS, json_encode($arg));
    return json_decode(curl_exec($c));
}

define("AK_SIZE", 20);
define("SK_SIZE", 40);

$ak = getenv("OSC_ACCESS_KEY");
$sk = getenv("OSC_SECRET_KEY");

$ak_sk = $ak.":".$sk;

$c = curl_init();

$hs[] = "Content-Type: application/json";

$obj = (array) [
    'Filters' => array(
        "ImageNames" => ["*Arch*"]
    )
];

curl_setopt($c, CURLOPT_HTTPHEADER, $hs);
curl_setopt($c, CURLOPT_RETURNTRANSFER, 1);
curl_setopt($c, CURLOPT_POSTFIELDS, "");
curl_setopt($c, CURLOPT_AWS_SIGV4, "osc");
curl_setopt($c, CURLOPT_USERPWD, $ak_sk);

$data = call($c, 'ReadImages', $obj);
$images = $data->Images;
foreach ($images as $img)
    echo json_encode($img->ImageName) , PHP_EOL;

// not really useful, but sending ImageNames to SG filter isn't useful either
unset($obj['Filters']['ImageNames']);
$obj['Filters']['SecurityGroupNames'] = ['default'];

$data = call($c, 'ReadSecurityGroups', $obj);
echo json_encode($data->SecurityGroups, JSON_PRETTY_PRINT), PHP_EOL;

curl_close($c);

?>
