# Markdown Practice

![Operating System Concepts](src/os10-cover.jpg)

```c
#include <stdio.h>
int main() {
  puts("Hello world!");
  return 0;
}
```

标准模型的拉格朗日量
$\mathcal{L}_{\mathrm{StandardModel}}$
$=-\frac{1}{2} \partial_{\nu} g_{\mu}^{a} \partial_{\nu} g_{\mu}^{a}-g_{s} f^{a b c} \partial_{\mu} g_{\nu}^{a} g_{\mu}^{b} g_{\nu}^{c}-\frac{1}{4} g_{s}^{2} f^{a b c} f^{a d e} g_{\mu}^{b} g_{\nu}^{c} g_{\mu}^{d} g_{\nu}^{e}+$
$\frac{1}{2} i g_{s}^{2}\left(\bar{q}_{i}^{\sigma} \gamma^{\mu} q_{j}^{\sigma}\right) g_{\mu}^{a}+\bar{G}^{a} \partial^{2} G^{a}+g_{s} f^{a b c} \partial_{\mu} \bar{G}^{a} G^{b} g_{\mu}^{c}-\partial_{\nu} W_{\mu}^{+} \partial_{\nu} W_{\mu}^{-}-$
$M^{2} W_{\mu}^{+} W_{\mu}^{-}-\frac{1}{2} \partial_{\nu} Z_{\mu}^{0} \partial_{\nu} Z_{\mu}^{0}-\frac{1}{2 c_{w}^{2}} M^{2} Z_{\mu}^{0} Z_{\mu}^{0}-\frac{1}{2} \partial_{\mu} A_{\nu} \partial_{\mu} A_{\nu}-\frac{1}{2} \partial_{\mu} H \partial_{\mu} H-$
$\frac{1}{2} m_{h}^{2} H^{2}-\partial_{\mu} \phi^{+} \partial_{\mu} \phi^{-}-M^{2} \phi^{+} \phi^{-}-\frac{1}{2} \partial_{\mu} \phi^{0} \partial_{\mu} \phi^{0}-\frac{1}{2 c_{w}^{2}} M \phi^{0} \phi^{0}-\beta_{h}\left[\frac{2 M^{2}}{g^{2}}+\right.$
$\left.\frac{2 M}{g} H+\frac{1}{2}\left(H^{2}+\phi^{0} \phi^{0}+2 \phi^{+} \phi^{-}\right)\right]+\frac{2 M^{4}}{g^{2}} \alpha_{h}-i g c_{w}\left[\partial_{\nu} Z_{\mu}^{0}\left(W_{\mu}^{+} W_{\nu}^{-}-\right.\right.$
$\left.W_{\nu}^{+} W_{\mu}^{-}\right)-Z_{\nu}^{0}\left(W_{\mu}^{+} \partial_{\nu} W_{\mu}^{-}-W_{\mu}^{-} \partial_{\nu} W_{\mu}^{+}\right)+Z_{\mu}^{0}\left(W_{\nu}^{+} \partial_{\nu} W_{\mu}^{-}-\right.$
$\left.\left.W_{\nu}^{-} \partial_{\nu} W_{\mu}^{+}\right)\right]-i g s_{w}\left[\partial_{\nu} A_{\mu}\left(W_{\mu}^{+} W_{\nu}^{-}-W_{\nu}^{+} W_{\mu}^{-}\right)-A_{\nu}\left(W_{\mu}^{+} \partial_{\nu} W_{\mu}^{-}-\right.\right.$
$\frac{1}{2} g^{2} W_{\mu}^{+} W_{\nu}^{-} W_{\mu}^{+} W_{\nu}^{-}+g^{2} c_{w}^{2}\left(Z_{\mu}^{0} W_{\mu}^{+} Z_{\nu}^{0} W_{\nu}^{-}-Z_{\mu}^{0} Z_{\mu}^{0} W_{\nu}^{+} W_{\nu}^{-}\right)+$
$g^{2} s_{w}^{2}\left(A_{\mu} W_{\mu}^{+} A_{\nu} W_{\nu}^{-}-A_{\mu} A_{\mu} W_{\nu}^{+} W_{\nu}^{-}\right)+g^{2} s_{w} c_{w}\left[A_{\mu} Z_{\nu}^{0}\left(W_{\mu}^{+} W_{\nu}^{-}-\right.\right.$
$\left.\left.W_{\nu}^{+} W_{\mu}^{-}\right)-2 A_{\mu} Z_{\mu}^{0} W_{\nu}^{+} W_{\nu}^{-}\right]-g \alpha\left[H^{3}+H \phi^{0} \phi^{0}+2 H \phi^{+} \phi^{-}\right]-$
$\frac{1}{8} g^{2} \alpha_{h}\left[H^{4}+\left(\phi^{0}\right)^{4}+4\left(\phi^{+} \phi^{-}\right)^{2}+4\left(\phi^{0}\right)^{2} \phi^{+} \phi^{-}+4 H^{2} \phi^{+} \phi^{-}+2\left(\phi^{0}\right)^{2} H^{2}\right]-$
$g M W_{\mu}^{+} W_{\mu}^{-} H-\frac{1}{2} g \frac{M}{c_{w}^{2}} Z_{\mu}^{0} Z_{\mu}^{0} H-\frac{1}{2} i g\left[W_{\mu}^{+}\left(\phi^{0} \partial_{\mu} \phi^{-}-\phi^{-} \partial_{\mu} \phi^{0}\right)-\right.$
$\left.W_{\mu}^{-}\left(\phi^{0} \partial_{\mu} \phi^{+}-\phi^{+} \partial_{\mu} \phi^{0}\right)\right]+\frac{1}{2} g\left[W_{\mu}^{+}\left(H \partial_{\mu} \phi^{-}-\phi^{-} \partial_{\mu} H\right)-W_{\mu}^{-}\left(H \partial_{\mu} \phi^{+}-\right.\right.$
$\left.\left.\phi^{+} \partial_{\mu} H\right)\right]+\frac{1}{2} g \frac{1}{c_{w}}\left(Z_{\mu}^{0}\left(H \partial_{\mu} \phi^{0}-\phi^{0} \partial_{\mu} H\right)-i g_{c_{w}}^{s_{w}^{2}} M Z_{\mu}^{0}\left(W_{\mu}^{+} \phi^{-}-W_{\mu}^{-} \phi^{+}\right)+\right.$
$i g s_{w} M A_{\mu}\left(W_{\mu}^{+} \phi^{-}-W_{\mu}^{-} \phi^{+}\right)-i g \frac{1-2 c_{w}^{2}}{2 c_{w}} Z_{\mu}^{0}\left(\phi^{+} \partial_{\mu} \phi^{-}-\phi^{-} \partial_{\mu} \phi^{+}\right)+$
$igs_{w} A_{\mu}\left(\phi^{+} \partial_{\mu} \phi^{-}-\phi^{-} \partial_{\mu} \phi^{+}\right)-\frac{1}{4} g^{2} W_{\mu}^{+} W_{\mu}^{-}\left[H^{2}+\left(\phi^{0}\right)^{2}+2 \phi^{+} \phi^{-}\right]-$
$\frac{1}{4} g^{2} \frac{1}{c_{w}^{2}} Z_{\mu}^{0} Z_{\mu}^{0}\left[H^{2}+\left(\phi^{0}\right)^{2}+2\left(2 s_{w}^{2}-1\right)^{2} \phi^{+} \phi^{-}\right]-\frac{1}{2} g^{2} \frac{s_{w}^{2}}{c_{w}} Z_{\mu}^{0} \phi^{0}\left(W_{\mu}^{+} \phi^{-}+\right.$
$\left.W_{\mu}^{-} \phi^{+}\right)-\frac{1}{2} i g^{2} \frac{s_{w}^{2}}{c_{w}} Z_{\mu}^{0} H\left(W_{\mu}^{+} \phi^{-}-W_{\mu}^{-} \phi^{+}\right)+\frac{1}{2} g^{2} s_{w} A_{\mu} \phi^{0}\left(W_{\mu}^{+} \phi^{-}+\right.$
$\left.W_{\mu}^{-} \phi^{+}\right)+\frac{1}{2} i g^{2} s_{w} A_{\mu} H\left(W_{\mu}^{+} \phi^{-}-W_{\mu}^{-} \phi^{+}\right)-g^{2} \frac{s_{w}}{c_{w}}\left(2 c_{w}^{2}-1\right) Z_{\mu}^{0} A_{\mu} \phi^{+} \phi^{-}-$
$g^{1} s_{w}^{2} A_{\mu} A_{\mu} \phi^{+} \phi^{-}-\bar{e}^{\lambda}\left(\gamma \partial+m_{e}^{\lambda}\right) e^{\lambda}-\bar{\nu}^{\lambda} \gamma \partial \nu^{\lambda}-\bar{u}_{j}^{\lambda}\left(\gamma \partial+m_{u}^{\lambda}\right) u_{j}^{\lambda}-$
$\bar{d}_{j}^{\lambda}\left(\gamma \partial+m_{d}^{\lambda}\right) d_{j}^{\lambda}+i g s_{w} A_{\mu}\left[-\left(\bar{e}^{\lambda} \gamma^{\mu} e^{\lambda}\right)+\frac{2}{3}\left(\bar{u}_{j}^{\lambda} \gamma^{\mu} u_{j}^{\lambda}\right)-\frac{1}{3}\left(\bar{d}_{j}^{\lambda} \gamma^{\mu} d_{j}^{\lambda}\right)\right]+$
$\frac{i g}{4 c_{w}} Z_{\mu}^{0}\left[\left(\bar{\nu}^{\lambda} \gamma^{\mu}\left(1+\gamma^{5}\right) \nu^{\lambda}\right)+\left(\bar{e}^{\lambda} \gamma^{\mu}\left(4 s_{w}^{2}-1-\gamma^{5}\right) e^{\lambda}\right)+\left(\bar{u}_{j}^{\lambda} \gamma^{\mu}\left(\frac{4}{3} s_{w}^{2}-\right.\right.\right.$
$\left.\left.\left.1-\gamma^{5}\right) u_{j}^{\lambda}\right)+\left(\bar{d}_{j}^{\lambda} \gamma^{\mu}\left(1-\frac{8}{3} s_{w}^{2}-\gamma^{5}\right) d_{j}^{\lambda}\right)\right]+\frac{i g}{2 \sqrt{2}} W_{\mu}^{+}\left[\left(\bar{\nu}^{\lambda} \gamma^{\mu}\left(1+\gamma^{5}\right) e^{\lambda}\right)+\right.$
$\left.\left(\bar{u}_{j}^{\lambda} \gamma^{\mu}\left(1+\gamma^{5}\right) C_{\lambda \kappa} d_{j}^{\kappa}\right)\right]+\frac{i g}{2 \sqrt{2}} W_{\mu}^{-}\left[\left(\bar{e}^{\lambda} \gamma^{\mu}\left(1+\gamma^{5}\right) \nu^{\lambda}\right)+\left(\bar{d}_{j}^{\kappa} C_{\lambda \kappa}^{\dagger} \gamma^{\mu}(1+\right.\right.$
$\left.\left.\left.\gamma^{5}\right) u_{j}^{\lambda}\right)\right]+\frac{i g}{2 \sqrt{2}} \frac{m_{e}^{\lambda}}{M}\left[-\phi^{+}\left(\bar{\nu}^{\lambda}\left(1-\gamma^{5}\right) e^{\lambda}\right)+\phi^{-}\left(\bar{e}^{\lambda}\left(1+\gamma^{5}\right) \nu^{\lambda}\right)\right]-$
$\frac{\partial}{2} \frac{m_{e}^{\lambda}}{M}\left[H\left(\bar{e}^{\lambda} e^{\lambda}\right)+i \phi^{0}\left(\bar{e}^{\lambda} \gamma^{5} e^{\lambda}\right)\right]+\frac{i g}{2 M \sqrt{2}} \phi^{+}\left[-m_{d}^{\kappa}\left(\bar{u}_{j}^{\lambda} C_{\lambda \kappa}\left(1-\gamma^{5}\right) d_{j}^{\kappa}\right)+\right.$
$m_{u}^{\lambda}\left(\bar{u}_{j}^{\lambda} C_{\lambda \kappa}\left(1+\gamma^{5}\right) d_{j}^{\kappa}\right]+\frac{i g}{2 M \sqrt{2}} \phi^{-}\left[m_{d}^{\lambda}\left(\bar{d}_{j}^{\lambda} C_{\lambda \kappa}^{\dagger}\left(1+\gamma^{5}\right) u_{j}^{\kappa}\right)-m_{u}^{\kappa}\left(\bar{d}_{j}^{\lambda} C_{\lambda \kappa}^{\dagger}(1-\right.\right.$
$\left.\left.\gamma^{5}\right) u_{j}^{\kappa}\right]-\frac{g}{2} \frac{m_{u}^{\lambda}}{M} H\left(\bar{u}_{j}^{\lambda} u_{j}^{\lambda}\right)-\frac{g}{2} \frac{m_{d}^{\lambda}}{M} H\left(\bar{d}_{j}^{\lambda} d_{j}^{\lambda}\right)+\frac{i g}{2} \frac{m_{u}^{\lambda}}{M} \phi^{0}\left(\bar{u}_{j}^{\lambda} \gamma^{5} u_{j}^{\lambda}\right)-$
$\frac{i g}{2} \frac{m_{d}^{\lambda}}{M} \phi^{0}\left(\bar{d}_{j}^{\lambda} \gamma^{5} d_{j}^{\lambda}\right)+\bar{X}^{+}\left(\partial^{2}-M^{2}\right) X^{+}+\bar{X}^{-}\left(\partial^{2}-M^{2}\right) X^{-}+\bar{X}^{0}\left(\partial^{2}-\right.$
$\left.\frac{M^{2}}{c_{w}^{2}}\right) X^{0}+\bar{Y} \partial^{2} Y+i g c_{w} W_{\mu}^{+}\left(\partial_{\mu} \bar{X}^{0} X^{-}-\partial_{\mu} \bar{X}^{+} X^{0}\right)+i g s_{w} W_{\mu}^{+}\left(\partial_{\mu} \bar{Y} X^{-}-\right.$
$\left.\partial_{\mu} \bar{X}^{+} Y\right)+i g c_{w} W_{\mu}^{-}\left(\partial_{\mu} \bar{X}-X^{0}-\partial_{\mu} \bar{X}^{0} X^{+}\right)+i g s_{w} W_{\mu}^{-}\left(\partial_{\mu} \bar{X}-Y-\right.$
$\left.\partial_{\mu} \bar{Y} X^{+}\right)+i g c_{w} Z_{\mu}^{0}\left(\partial_{\mu} \bar{X}^{+} X^{+}-\partial_{\mu} \bar{X}^{-} X^{-}\right)+i g s_{w} A_{\mu}\left(\partial_{\mu} \bar{X}^{+} X^{+}-\right.$
$\left.\partial_{\mu} \bar{X}-X^{-}\right)-\frac{1}{2} g M\left[\bar{X}+X^{+} H+\bar{X}^{-} X^{-} H+\frac{1}{c_{w}^{2}} \bar{X}^{0} X^{0} H\right]+$
$\frac{1-2 c_{w}^{2}}{2 c_{w}} i g M\left[\bar{X}^{+} X^{0} \phi^{+}-\bar{X}^{-} X^{0} \phi^{-}\right]+\frac{1}{2 c_{w}} i g M\left[\bar{X}^{0} X^{-} \phi^{+}-\bar{X}^{0} X^{+} \phi^{-}\right]+$
$ig M s_{w}\left[\bar{X}^{0} X^{-} \phi^{+}-\bar{X}^{0} X^{+} \phi^{-}\right]+\frac{1}{2} i g M\left[\bar{X}^{+} X^{+} \phi^{0}-\bar{X}^{-} X^{-} \phi^{0}\right]$
