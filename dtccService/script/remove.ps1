# create a log file
$log = 'log.txt'
'starting remove script' | Set-Content $log

if (!$args)
{
	Write-Host "service name missing"
	throw
}

$name = $args[0]

"Trying to remove service $name" | Add-Content $log

Try
{
    if (Get-Service $name -ErrorAction Stop)
    {
        sc.exe stop $name -ErrorAction Continue | Add-Content $log
		"successfully stopped Windows service $name" | Add-Content $log

	    sc.exe delete $name -ErrorAction Stop | Add-Content $log
        "successfully removed Windows service $name" | Add-Content $log
    }
}
Catch [SystemException]
{
	if ($_.Exception.Message -eq "Cannot find any service with service name '" + $name + "'.")
	{
		"The service $name is not registered" | Add-Content $log
	}
	else
	{
		$_.Exception.GetType().FullName, $_.Exception.Message | Add-Content $log
		$_.Exception|format-list -force | Add-Content $log
		throw
	}
}
Catch [Exception]
{
    $_.Exception.GetType().FullName, $_.Exception.Message | Add-Content $log
    $_.Exception|format-list -force | Add-Content $log
    throw
}