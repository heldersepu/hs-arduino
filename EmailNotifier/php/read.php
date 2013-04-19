<?php
include_once 'pop3_functions.php';

$host = "pop.googlemail.com";
$port = "995";
$user = "yourEmail@gmail.com";
$pass = "passw";

$mbox = pop3_login($host,$port,$user,$pass,"INBOX",true);
if ($mbox)
{
    echo "<h1>Connected</h1>\n";
    imap_close($mbox);
} else
{
    echo "<h1>FAIL!</h1>\n";
}
?>

