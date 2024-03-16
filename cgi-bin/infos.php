
<?php



// Affiche toutes les informations, comme le ferait INFO_ALL
//phpinfo();
$arr = get_defined_vars();
print_r($arr);

//$a = print_r(var_dump($GLOBALS),1);
//echo '<pre>';
//echo htmlspecialchars($a);
//echo '</pre>';

// Affiche uniquement le module d'information.
// phpinfo(8) fournirait les mêmes informations.
//phpinfo(INFO_MODULES);

//echo "Path: " . $_ENV['Path'];

// foreach ($_ENV as $k=>$v)
//   echo $k . " => " . $v . "<br>";
?>

