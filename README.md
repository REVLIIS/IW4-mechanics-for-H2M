# IW4 mechanics for H2M-mod

This plugin aims to bring MW2 (2009) gun glitches/mechanics to H2M

## ✅ Currently included:

* Wrist twist (+pc variant)
* Righty tighty
* Some lesser known mw2 sprint-related mechanics

<sub>
I also added a plugin to fix still swaps that were originally meant to be patched out before the h2m takedown. This plugin is in a separate file, in case you prefer to keep the still swaps broken. However, my plugins will be built with the fix in mind.
<br>
This fix may not work if you're using a different version of the exe. I created the patch for the most common version of the .exe <br>(SHA-256 hash: 9dcfda29748e29e806119cb17847bb3617c188b402ed743bd16e770401f9e127).
</sub> 

## 🛠️ Planned for the near future:
* Infinite faydes
* Jitters
* Tac flips and barrel rolls (probably can't be done with just code, will probably require custom anims)

#

**⚠️ Warning: To use this plugin on a server, both the server and the client must have the plugin running!** You can join servers that don't have the plugin without any issues, but the mechanics won't work correctly.

## 🚀 How to use

<sub>Before you start, I highly recommend using my [H2M Plugin loader](https://github.com/REVLIIS/H2M-Plugin-Loader), which will automatically load plugins when you start h2m.</sub>

1. Go to the [Releases](https://github.com/REVLIIS/IW4-mechanics-for-H2M/releases) section in the sidebar and download the latest `.zip` file.

### Using my H2M Plugin loader:
2. Unzip the contents of the `.zip` in your plugins folder
![image](https://github.com/user-attachments/assets/82e6bd07-d289-4eda-b3da-c0e7c0c317ae)
3. Run h2m-mod.exe (or your server's startup .bat file if you're a server hoster) and if you setup everything correctly, your console window should look something like this:

### Alternatively you can use your own injector:
2. Unzip the contents of the `zip` somewhere.
3. Inject the DLL(s) at H2M startup (Most consistent results when injecting after the splashscreen goes away and the game window comes up)

