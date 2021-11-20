<?php
include "config.php";
$id=$_REQUEST['id'];
if($id)
{
$status=$_REQUEST['status'];

$sql = "UPDATE `url_service` SET `Status`=".$status.",`match_id`=0 WHERE `id`=".$id;	
//echo $sql;
if ($result = mysqli_query($conn, $sql)) {

} 
else {

}
mysqli_close($conn);
}
?>