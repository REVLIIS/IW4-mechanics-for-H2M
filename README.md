# IW4 mechanics for H2M-mod

This plugin aims to bring MW2 (2009) gun glitches/mechanics to H2M

## ✅ Currently included:

* Wrist twist (+pc variant)
* Righty tighty
* Some lesser known mw2 sprint-related mechanics

<sub>Works with +reload and +usereload binds (deliberately).</sub>

## 🛠️ Planned for the near future:
* Infinite faydes
* Jitters
* Tac flips and barrel rolls (probably can't be done with just code, will probably require custom anims)

## ⚠️ Warning
**To use this plugin on a server, both the SERVER and the CLIENT(s) must have the plugin running!**<br>
If you have plugins running that the server isn't running, the mechanics will not work properly. 
Similarly, if the server has plugins running that the client doesn't have running, the mechanics won't work properly for that client.

## 🚀 How to setup

<sub>Before you start, I highly recommend using my [H2M Plugin loader](https://github.com/REVLIIS/H2M-Plugin-Loader), which will automatically load plugins when you start h2m.</sub>

1. Go to the [Releases](https://github.com/REVLIIS/IW4-mechanics-for-H2M/releases) section in the sidebar and download the latest `.zip` file. Or click [here](https://github.com/REVLIIS/IW4-mechanics-for-H2M/releases/download/v1.0.0/IW4.Mechanics.for.H2M.v1.0.0.zip) for a direct download.

### Using my H2M Plugin loader:
2. Unzip the contents of the `.zip` in your plugins folder
![image](https://github.com/user-attachments/assets/82e6bd07-d289-4eda-b3da-c0e7c0c317ae)
3. Run h2m-mod.exe (or your server's startup .bat file if you're a server hoster) and if you setup everything correctly, your console window should look something like this:

### Alternatively you can use your own injector:
2. Unzip the contents of the `zip` somewhere.
3. Inject the DLL(s) at H2M startup (Most consistent results when injecting after the splashscreen goes away and the game window comes up)


# Additional info
<sub>
I also added a plugin to fix still swaps that were originally meant to be patched out before the h2m takedown. This plugin is in a separate file, in case you prefer to keep the still swaps broken. However, my plugins will be built with the fix in mind.
<br>
This fix may not work if you're using a different version of the exe. I created the patch for the most common version of the .exe <br>(SHA-256 hash: 9dcfda29748e29e806119cb17847bb3617c188b402ed743bd16e770401f9e127).
</sub> 


<br><br>
<details>
<summary>Videos</summary>
<br>

[Wrist Twist](https://github.com/user-attachments/assets/ee493a6c-b067-46ef-a204-7548970d2157)

How to:
Same as MW2 2009, shoot bullet(s) out of left gun, reload and sprint at the same time

<br>

[Righty Tighty](https://github.com/user-attachments/assets/fa64f1e7-adb5-4248-9713-c89ce7b4bfad)

How to:
Same as MW2 2009, shoot bullet(s) out of right gun, tap sprint and tap reload at the same time.

## Porting IW4's sprint checks brought some other niche stuff back as well:

[PC Wrist twist](https://github.com/user-attachments/assets/1a97595f-f9d6-49ba-b4d8-1c3302d896c2)

[PC rmb sprint](https://github.com/user-attachments/assets/9dc45bba-f1fb-4ac5-9ab3-69efcae98bce)

And finally the still swap patch removes still swaps by just holding sprint and switching weaps.

[Still swap patch](https://github.com/user-attachments/assets/cf10c3d7-1239-447e-95ce-98a931479de4)

This also fixes having to hold sprint to glide sprint drop anims

[Glide out fix](https://github.com/user-attachments/assets/5fff285d-24b7-4aa4-83d8-18d15ff6ac04)

</details>
