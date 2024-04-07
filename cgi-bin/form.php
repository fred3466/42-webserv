<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Insert title here</title>
</head>
<body>
<?php 
$input_data = file_get_contents("php://stdin");
echo("input_data=<hr>$input_data<hr>");

?>
HTTP_RAW_POST_DATA : <?php var_dump($HTTP_RAW_POST_DATA); ?>
<br>
Bonjour, <?php echo htmlspecialchars($_POST['nom']); ?>.
Tu as <?php echo (int)$_POST['age']; ?> ans.

<br>
<form action="form.php" method="post">
   <label>Votre nom :</label>
   <input name="nom" id="nom" type="text" value="fred"/>
<br>
   <label>Votre âge :</label>
   <input name="age" id="age" type="number" value="25"/></p>

   <button type="submit">Valider</button>
</form>

<?php
//$arr = get_defined_vars();
//echo(( nl2br(print_r($arr,true))));
?>

</body>
</html>