using UnityEngine;

public class BackgroundScroller : MonoBehaviour
{
    [Header("Track Properties")]
    [SerializeField]
    Track track;

    public float scrollSpeed = 0f; // Speed at which the background scrolls

    // private
    private Vector2 startPosition; // Store the initial position
    private float width; // Width of the background

    float offset = 0f;

    void Start()
    {
        startPosition = transform.position; // Store the initial position
        width = GetComponent<SpriteRenderer>().bounds.size.x; // Get the width of the background
    }

    void Update()
    {
        offset += Time.deltaTime;
        // Calculate the new position for the background
        float newPositionX = offset * scrollSpeed * track.speed;
        //if (newPositionX > width) newPositionX = width;

        // If the background has moved far enough, reset its position
        if (newPositionX >= width)
        {
            //startPosition.x += width;
            //offset = ((newPositionX - width) / track.speed) / scrollSpeed;
            //newPositionX = newPositionX - width;
            newPositionX -= width;
            offset = (newPositionX / track.speed) / scrollSpeed;
        }

        transform.position = startPosition + Vector2.left * newPositionX;

        //if (!track.audioManager.sounds[0].source.isPlaying) return;
        
    }
}
