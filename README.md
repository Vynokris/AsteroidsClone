# AsteroidsClone

This project is a clone of the retro game "Asteroids" done in C using `raylib` and `raymath`.


<br>

## Recreated features
<ul>
<li> Player movement </li>
    <ul>
    <li> Keyboard </li>
        <ul>
        <li> Press the right and left arrows to turn. </li>
        <li> Press the up arrow to accelerate. </li>
        <li> The player has a forward velocity that diminishes when he isn't accelerating. </li>
        </ul>
    <li> Gamepad </li>
        <ul>
        <li> Use the right joystick to turn. </li>
        <li> Use the left joystick to move. </li>
        <li> Press R2 to shoot. </li>
        </ul>
    </ul>
<li> Player HP </li>
    <ul>
    <li> The player has 3 health points. </li>
    <li> The player looses HP when he touches an asteroid and is teleported to the center of the screen with a temporary invulnerability. </li>
    </ul>
<li> Shooting </li>
    <ul>
    <li> Press space to shoot a projectile that can destroy asteroids. </li>
    </ul>
<li> Asteroids </li>
    <ul>
    <li> Asteroids spawn outside of the screen and have a random direction. </li>
    <li> When an asteroid is shot, it breaks into two smaller asteroids. </li>
    <li> There are three asteroid sizes: large, medium and small. </li>
    <li> When a small asteroid is shot, it is destroyed. </li>
    </ul>
<li> Screen wrapping </li>
    <ul>
    <li> When the player or an asteroid goes out of the screen, it comes back on the other side. </li>
    </ul>
</ul>


<br>

## Added features
<ul>
<li> User interface </li>
    <ul>
    <li> The UI was reworked to be more modern. </li>
    <li> All information is at the center of the screen, rendered under all game objects. </li>
    <li> Some screen shake has been added upon asteroid destruction. </li>
    </ul>
<li> Asteroids score yeild </li>
    <ul>
    <li> Asteroids now have a number on them, that corresponds to the amount of score they yeild. </li>
    <li> The score yeild of an asteroid is unique and inferior to 64. </li>
    </ul>
<li> Amount of bullets </li>
    <ul>
    <li> In the original game, the player had infinite bullets. However, in this game, the player only has 3 bullets. </li>
    <li> This was done to try to force the player to move around and not be an automatic turret in the center of the screen. </li>
    <li> The player's bullets replenish when they touch the edge of the screen or an asteroid. </li>
    </ul>
<li> Score multiplier </li>
    <ul>
    <li> A score multiplier was implemented to reward player accuracy. </li>
    <li> When the player hits an asteroid, he increments his multiplier. </li>
    <li> When the player misses and hits the edge of the screen, his multiplier is reset to 1. </li>
    <li> The multiplier works only in powers of 2 and its maximum is 16. </li>
    </ul>
<li> Game timer </li>
    <ul>
    <li> A timer was added, that counts the duration of the game in seconds. </li>
    </ul>
<li> Highscore board </li>
    <ul>
    <li> On the game over screen, the player can view his highscores and the duration of the game they were optained in. </li>
    </ul>
</ul>
