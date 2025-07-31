# playing with K-Sortable Globally Unique IDs

$ErrorActionPreference = [System.Management.Automation.ActionPreference]::Stop
Set-StrictMode -Version Latest

function New-KSUID
{
    param (
        [DateTime]$Date = (Get-Date)
    )

    # Epoch base (KSUID uses 2014-05-13 00:00:00 UTC)
    [DateTime]$ksuidEpoch = [DateTime]::Parse("2014-05-13T00:00:00Z").ToUniversalTime()

    # Seconds since KSUID epoch
    [int]$timestamp = [int][Math]::Floor(($Date.ToUniversalTime() - $ksuidEpoch).TotalSeconds)

    # Convert timestamp to 4-byte array (big endian)
    [Byte[]]$timestampBytes = [BitConverter]::GetBytes([UInt32]$timestamp)
    if ([BitConverter]::IsLittleEndian)
    {
        [Array]::Reverse($timestampBytes)
    }

    # Generate 16 random bytes
    [Byte[]]$randomBytes = New-Object byte[] 16
    [System.Security.Cryptography.RandomNumberGenerator]::Create().GetBytes($randomBytes)

    # Combine timestamp + random bytes
    [Byte[]]$bytes = $timestampBytes + $randomBytes

    # Encode to base64 with URL-safe parameters (so using - and _) as it appears to be best practice with a UID
    [System.String]$ksuidBase64 = [Convert]::ToBase64String($bytes).TrimEnd('=').Replace('+','-').Replace('/','_')

    return $ksuidBase64
}

### Testing ###
#New-KSUID

1..5 | ForEach-Object {
    Start-Sleep -Milliseconds 100
    New-KSUID
} | Sort-Object | ForEach-Object { Write-Host $_ }