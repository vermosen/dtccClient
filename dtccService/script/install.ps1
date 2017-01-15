if (!$args)
{
	Write-Host "service name missing"
	throw
}

$name = $args[0]
$path = $args[1]

Write-Host "Trying to install service" $name

Try
{
    if (Get-Service $name -ErrorAction Stop)
    {
		Write-Host "service" $name "hasn't been removed"
		throw
    }
	else
	{
	    sc.exe create $name binPath= $path
		sc.exe start $name

        Write-Host "successfully added Windows service " $name
	}
}
Catch [Exception]
{
    echo $_.Exception.GetType().FullName, $_.Exception.Message
    echo $_.Exception|format-list -force
    throw
}