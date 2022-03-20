<?php
//echo $_GET['param1'];

$servername = "localhost";

// REPLACE with your Database name
$dbname = "";
// REPLACE with Database user
$username = "";
// REPLACE with Database user password
$password = "";

$conn = mysqli_connect($servername, $username, $password, $dbname);
mysqli_set_charset($conn,'utf8');
$code=$_GET['param1'];
$sql="SELECT login_check_in.student_code,student_name,num_attend,num_late,last_check_in 
FROM login_check_in INNER JOIN student_info ON login_check_in.student_code=student_info.student_code 
WHERE class_code= '$code' ";
$data = mysqli_query($conn,$sql);




class dataDetail {
    function __construct($student_code,$student_name,$num_attend,$num_late,$last_check_in) {
        $this->StudentCode= $student_code;
        $this->StudentName= $student_name;
        $this->NumAttend = $num_attend;
        $this->NumLate=$num_late;
        $this->LastCheckIn=$last_check_in;
    }
}

$dataArray=array();

while ($row=mysqli_fetch_assoc($data))
{
    $holder = new dataDetail($row['student_code'],$row['student_name'],$row['num_attend'],$row['num_late'], $row['last_check_in']);
    array_push($dataArray,$holder);
    //array_push($dataArray,new dataDetail("1","1","1","1","1"));
    //array_push($dataArray,$row['class_code']);
}
echo json_encode($dataArray);



?>