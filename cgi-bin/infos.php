#!/usr/bin/php
<?php



// Affiche toutes les informations, comme le ferait INFO_ALL
// phpinfo(INFO_ENVIRONMENT|INFO_VARIABLES|INFO_CONFIGURATION|INFO_GENERAL);
phpinfo(INFO_ENVIRONMENT|INFO_VARIABLES);
$arr = get_defined_vars();
print_r($arr);
// phpinfo();

$a = print_r(var_dump($GLOBALS),1);
echo '<pre>';
echo htmlspecialchars($a);
echo '</pre>';

// // Affiche uniquement le module d'information.
// // phpinfo(8) fournirait les mêmes informations.
// //phpinfo(INFO_MODULES);


foreach ($_ENV as $k=>$v)
    echo htmlspecialchars($k . " => " . $v) . "<br>";
?>

