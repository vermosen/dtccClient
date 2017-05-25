# create a log file
$log = 'log.txt'
'starting install script' | Set-Content $log

if (!$args)
{
	Write-Host "service name missing"
	throw
}

$name = $args[0]
$path = $args[1]
$arg  = $args[2]
$user = $arg[3]
$password = $arg[4]

"Trying to install service $name from path $path with arguments $arg" | Add-Content $log

Try
{
    if (Get-Service $name -ErrorAction Continue)
    {
		"service $name hasn't been removed" | Add-Content $log
		throw
    }
	else
	{
		"trying to create service ($name)" | Add-Content $log
	    sc.exe create $name binPath= "$path $arg" | Add-Content $log
		"trying to start service ($name)" | Add-Content $log
		sc.exe start $name | Add-Content $log

        "successfully added Windows service ($name)" | Add-Content $log
	}
}
Catch [Exception]
{
	"An exception has been thrown" | Add-Content $log
    $_.Exception.GetType().FullName, $_.Exception.Message | Add-Content $log
    $_.Exception|format-list -force | Add-Content $log
    throw
}