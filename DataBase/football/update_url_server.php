<?php
include "config.php";
// Query the table to get the queue array
$return_arr = array();
if($_GET['id'] && $_GET['url'] )
{
	
	
	

$m_id=$_GET['id'];
$path=$_GET['url'];
$sql = "UPDATE `ready_match` SET `url`='".$path."' WHERE `id`=$m_id";
$result = mysqli_query($conn, $sql);

if ($result) {
 $return_arr['message']="ok";
} 
else 
{
 $return_arr['message']="not updated";
}

// Close connection
mysqli_close($conn);
}
else
{
	$return_arr['message']="missing inputs";
}
echo  json_encode($return_arr);

?>


