<?php
include "config.php";
$id_team=$_REQUEST['id_team'];
$sql = "DELETE FROM `match_making` WHERE `Team_id` ='".$id_team."'";

	$return_arr['id'] ="Ok";


mysqli_close($conn);

echo json_encode($return_arr);
?>
