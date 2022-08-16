# GPU

GPU (Graphics Processor Unit) рисует двухмерные примитивы в видеопамяти и выводит картинку на TV.

Дополнительно GPU занимается мастерингом видеосигнала (например управляет Color Burst), хотя непосредственно генерацию видеосигнала производит отдельная микросхема-ЦАП, на базе цифровых выходных сигналов RGB, идущих с GPU.

Управление GPU со стороны CPU осуществляется всего двумя IO-портами `GP0` и `GP1`, а также DMA, для передачи дисплейных списков (display lists, DL).

GPU построен по такой же технологии, что и CPU (standard cells + custom blocks):

[[File:GPU chip lowres.jpg|300px]] [[File:GPU standard cells.jpg|300px]]

- Старые версии материнок (PU-7 и старые PU-8) использовали 2 чипа GPU: 160pin CXD8514Q (IC203) и 64pin CXD2923AR (IC207). Одна из них занималась рисованием примитивов (более крупная 160-контактная), а вторая очевидно выводом картинки из видеопамяти на ТВ (64-контактная). Мануалов у нас нет, поэтому узнать точнее можно только по фотографиям материнской платы (отследить дорожки).
- Все остальные версии использовали 208pin версии GPU, индекс остался прежним - IC203.

{| cellspacing="0" border="1"
|-
! style="background:rgb(204,204,204)" align="center" |PCB
! style="background:rgb(204,204,204)" align="center" |[[PU-7]] / older [[PU-8]]
! style="background:rgb(204,204,204)" align="center" |newer [[PU-8]]
! style="background:rgb(204,204,204)" align="center" |[[PU-18]]
! style="background:rgb(204,204,204)" align="center" |[[PU-20]]
! style="background:rgb(204,204,204)" align="center" |[[PU-22]]
! style="background:rgb(204,204,204)" align="center" |[[PU-23]]
! style="background:rgb(204,204,204)" align="center" |[[PM-41]]
! style="background:rgb(204,204,204)" align="center" |[[PM-41(2)]]
|-
|IC203
|[[File:PU7 gpu package.jpg|100px]]
|[[File:NewPU8 gpu package.jpg|100px]]
|[[File:PU18 gpu package.jpg|100px]]
|???
|[[File:PU22 gpu package.jpg|100px]]
|[[File:PU23 gpu package.jpg|100px]]
|[[File:PM41 gpu package.jpg|100px]]
|[[File:PM412 gpu package.jpg|100px]]
|-
|CXD
|8514Q/2923AR
|8561Q
|8561BQ
|???
|8561CQ
|8561CQ
|8561CQ
|???
|}

## Аппаратный интерфейс (старый GPU 160-pin)

Cтарый GPU состоящий из двух чипов (CXD 8514Q/2923AR) работает следующим образом:

- Интерфейс между GPU и CPU не претерпел изменений в последующих версиях.
- Большая микросхема CXD 8514Q (160 контактов) занимается отрисовкой примитивов в специальную двух-портовую DRAM. Соответственно она также содержит DRAM Refresh логику. Картина немного усложняется тем, что VRAM поделена на два банка (2 микросхемы).
- Маленькая микросхема CXD 2923AR (64 контакта) занимается выборкой пикселей из VRAM и представляет собой RGB DAC
