using System;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    #region variables

    public enum State // Track player state
    {
        idle = 0,
        lane1,
        lane2,
        lane3,
        hurt,
        fall
    }

    public enum Anim // Track animation state
    {
        run = 0,
        slide,
        cJump,
        jump,
        kick,
        hold_blend,
        hold_loop,
        hurt
    }

    public Dictionary<Anim, string> anims;

    Dictionary<string, int> aIndex; // map anim name to anim index within animator

    Dictionary<Anim, State> aToS;

    public State state = State.idle;

    public Anim aStatePrev = Anim.run;
    public Anim aState = Anim.run;

    private Rigidbody2D rb;

    private Queue<Anim> aQueue;

    AnimatorStateInfo aStateInfo;

    [Header("Fields")]
    [SerializeField]
    Transform lane1T;
    [SerializeField]
    Transform lane2T;
    [SerializeField]
    Transform lane3T;

    [SerializeField]
    PhysicsMaterial2D note;
    [SerializeField]
    PhysicsMaterial2D ground;

    [SerializeField]
    private Animator anim;

    #endregion

    private void Start()
    {
        rb = GetComponent<Rigidbody2D>(); 
        anims = new Dictionary<Anim, string>();
        anims.Add(Anim.run, "run");
        anims.Add(Anim.slide, "slide");
        anims.Add(Anim.cJump, "cJump");
        anims.Add(Anim.jump, "jump");
        anims.Add(Anim.kick, "kick");
        anims.Add(Anim.hold_blend, "hold_blend");
        anims.Add(Anim.hold_loop, "hold_loop");

        aToS = new Dictionary<Anim, State>();
        aToS.Add(Anim.run, State.idle);
        aToS.Add(Anim.slide, State.lane3);
        aToS.Add(Anim.cJump, State.lane2);
        aToS.Add(Anim.jump, State.lane1);

        state = State.idle;

        aQueue = new Queue<Anim>();

        aStateInfo = anim.GetCurrentAnimatorStateInfo(0);

        // map anim name to anim index within animator
        aIndex = new Dictionary<string, int>();
        for (int i = 0; i < anim.runtimeAnimatorController.animationClips.Length; i++)
        {
            aIndex.Add(anim.runtimeAnimatorController.animationClips[i].name, i);
        }
    }

    private void Update()
    {
        // if (Input.GetKey(KeyCode.Alpha0)) anim.Play("run");
        // if (Input.GetKey(KeyCode.Alpha1)) anim.Play("jump");
        // if (Input.GetKey(KeyCode.Alpha2)) anim.Play("cJump");
        // if (Input.GetKey(KeyCode.Alpha3)) anim.Play("hold_blend");
        // if (Input.GetKey(KeyCode.Alpha4)) anim.Play("hold_loop");
        // if (Input.GetKey(KeyCode.Alpha5)) anim.Play("kick");

        UpdateAnim();
    }

    void updateA(Anim a)
    {
        switch (aState)
        {
            case Anim.run:
            case Anim.kick:
            case Anim.hold_blend:

            break;

            default:
            if (aState != a) aStatePrev = aState;
            break;
        }

        aState = a;
    }

    void UpdateAnim()
    {
        // Reactionary / Blend Animations

        if (aStateInfo.normalizedTime >= 1f && aQueue.Count > 0)
        {
            print("0");
            updateA(aQueue.Dequeue());
        }

        switch(aState) // todo: fix (kick and hold animations not running)
        {
            case Anim.kick:
                if (aStateInfo.IsName("kick") && aStateInfo.normalizedTime >= 1f)
                {
                    state = State.fall;
                    updateA(aStatePrev);
                }
                break;
            case Anim.hold_blend:
                if (aStateInfo.IsName("hold_blend") && aStateInfo.normalizedTime >= 1f) updateA(Anim.hold_loop);
                break;
        }

        anim.Play(anims[aState]);
    }

    void Hurt() //todo: hurt player
    {
        state = State.hurt;
    }

    void OnTriggerEnter2D(Collider2D other) // when getting hit by note
    {
        if (other.sharedMaterial == note) Hurt();
    }

    void OnTriggerStay2D(Collider2D other)
    {
        if (state == State.fall && other.sharedMaterial == ground)
        {
            state = State.idle;
            updateA(Anim.run);
        }
    }

    void Jump(int hit) // jump to lane 1
    {
        state = State.lane1;
        updateA(Anim.jump);
        switch (hit)
        {
            case 0:
            anim.runtimeAnimatorController.animationClips[aIndex[anims[Anim.jump]]].wrapMode = WrapMode.Loop;
            break;
            case 1:
            anim.runtimeAnimatorController.animationClips[aIndex[anims[Anim.jump]]].wrapMode = WrapMode.Once;
            aQueue.Enqueue(Anim.kick);
            break;
            case 2:
            anim.runtimeAnimatorController.animationClips[aIndex[anims[Anim.jump]]].wrapMode = WrapMode.Once;
            aQueue.Enqueue(Anim.hold_blend);
            break;
        }

        this.transform.position = lane1T.position; //print("1");
    }

    void CrouchJump(int hit) // jump to lane 2
    {
        state = State.lane2;
        updateA(Anim.cJump);
        switch (hit)
        {
            case 0:
            anim.runtimeAnimatorController.animationClips[aIndex[anims[Anim.cJump]]].wrapMode = WrapMode.Loop;
            break;
            case 1:
            anim.runtimeAnimatorController.animationClips[aIndex[anims[Anim.cJump]]].wrapMode = WrapMode.Once;
            aQueue.Enqueue(Anim.kick);
            break;
            case 2:
            anim.runtimeAnimatorController.animationClips[aIndex[anims[Anim.cJump]]].wrapMode = WrapMode.Once;
            aQueue.Enqueue(Anim.hold_blend);
            break;
        }

        this.transform.position = lane2T.position; //print("2");
    }

    void Slide(int hit)  // slide to lane 3
    {
        state = State.lane3;
        updateA(Anim.slide);

        switch (hit)
        {
            case 0:
            anim.runtimeAnimatorController.animationClips[aIndex[anims[Anim.slide]]].wrapMode = WrapMode.Loop;
            break;
            case 1:
            //anim.runtimeAnimatorController.animationClips[aIndex[anims[Anim.slide]]].wrapMode = WrapMode.Once;
            //updateA(Anim.kick);
            break;
            case 2:
            //anim.runtimeAnimatorController.animationClips[aIndex[anims[Anim.slide]]].wrapMode = WrapMode.Once;
            //updateA(Anim.hold_blend);
            break;
        }

        this.transform.position = lane3T.position; //print("3");
    }

    void Reset() // reset sprite + scale
    {
        // Reset to normal state
        state = State.fall;
        updateA(aStatePrev);
    }

    public void MoveTo(int i, int hit) // public interface (to move to lanes)
    {
        //print(i);
        switch (i)
        {
            case 0:
            Reset();
            break;
            case 1:
            Jump(hit);
            break;
            case 2:
            CrouchJump(hit);
            break;
            case 3:
            Slide(hit);
            break;
        }
    }
}
