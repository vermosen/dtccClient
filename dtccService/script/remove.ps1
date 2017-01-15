if (!$args)
{
	Write-Host "service name missing"
	throw
}

$name = $args[0]

Write-Host "Trying to remove service" $name

Try
{
    if (Get-Service $name -ErrorAction Stop)
    {
        sc.exe stop $name -ErrorAction Stop
	    sc.exe delete $name -ErrorAction Stop

        Write-Host "successfully removed Windows service " $name
    }
}
Catch [SystemException]
{
	if ($_.Exception.Message -eq "Cannot find any service with service name '" + $name + "'.")
	{
		Write-Host "The service" $name "is not registered"
	}
	else
	{
		echo $_.Exception.GetType().FullName, $_.Exception.Message
		echo $_.Exception|format-list -force
		throw
	}
}
Catch [Exception]
{
    echo $_.Exception.GetType().FullName, $_.Exception.Message
    echo $_.Exception|format-list -force
    throw
}