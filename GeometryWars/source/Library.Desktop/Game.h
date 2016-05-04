#pragma once

#include "SList.h"
#include "Vector.h"
#include "Stack.h"
#include "Hashmap.h"
#include "Graph.h"

#include "GameClock.h"
#include "GameTime.h"

#include "Renderer.h"
#include "RenderDevice.h"
#include "Animator.h"
#include "AnimationState.h"
#include "AnimationFrame.h"

#include "RTTI.h"
#include "Datum.h"
#include "Scope.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "GameObject.h"

#include "Action.h"
#include "Event.h"

#include "ActionList.h"
#include "ActionListSwitch.h"
#include "ActionExpression.h"
#include "ActionIfThenElse.h"
#include "ActionWhileLoop.h"
#include "ActionCreateEntity.h"
#include "ActionDestroyEntity.h"
#include "ActionCreateEntityFromFile.h"
#include "ActionLoadSector.h"

#include "ActionEvent.h"
#include "Reaction.h"
#include "ReactionAttributed.h"

#include "Image.h"
#include "SpriteRenderer.h"
//#include "PolygonRenderer.h"
#include "CircleColliderComponent.h"

#include "ActionLoadMusic.h"
#include "ActionPlayMusic.h"
#include "ActionTogglePauseMusic.h"
#include "ActionStopMusic.h"


#include "SharedDataTable.h"
#include "XmlParseMaster.h"
#include "XmlParseHelperDefine.h"
#include "XmlParseHelperDefineSector.h"
#include "XmlParseHelperWorld.h"
#include "XmlParseHelperSector.h"
#include "XmlParseHelperEntity.h"
#include "XmlParseHelperGameObject.h"
#include "XmlParseHelperAction.h"
#include "XmlParseHelperTable.h"
#include "XmlParseHelperPrimitives.h"
#include "XmlParseHelperNameValue.h"
#include "XmlParseHelperActionSwitch.h"
#include "XmlParseHelperActionExpression.h"
#include "XmlParseHelperReaction.h"
#include "XmlParseHelperActionEvent.h"
#include "XmlParseHelperActionIf.h"
#include "XmlParseHelperActionWhile.h"
#include "XmlParseHelperBeginPlay.h"
#include "XmlParseHelperSprite.h"
#include "XmlParseHelperPolygon.h"
#include "XmlParseHelperImage.h"
#include "XmlParseHelperOnDestroy.h"
#include "XmlParseHelperAnimator.h"
#include "XmlParseHelperAnimationState.h"
#include "XmlParseHelperAnimationFrame.h"
#include "XmlParseHelperCircleColliderComponent.h"

namespace Library
{
	/**
	 *	Game class which contains the world, manages the game clock
	 *	Call Start() before the game loop
	 *	Call Update() in the game loop
	 *	Call Destroy() after the game loop
	 */
	class Game final
	{
	public:

		/**
		 *	parameterless constructor, initializes the members
		 */
		Game();

		/**
		 *	disallow copy constructor
		 */
		Game(const Game& rhs) = delete;

		/**
		 *	destructor
		 */
		~Game();

		/**
		 *	disallow assignment operator
		 */
		Game& operator=(const Game& rhs) = delete;

		/**
		*	Init - All inits required, go here.
		*/
		void Init();

		/**
		 *	Getter for the game World
		 *	@return reference to World
		 */
		World& GetWorld();

		/**
		 *	Getter for the XML parse master of the Game
		 *	@return reference to Xml Parse Master
		 */
		XmlParseMaster& ParseMaster();

		/**
		 *	Getter of Game time
		 *	@return const reference to game time
		 */
		const GameTime& GetGameTime() const;

		/**
		 *	Starts the game timer, calls BeginPlay on World
		 */
		void Start();

		/**
		 *	AddHelpers - Add helpers to parse master.
		 */
		void AddHelpers();

		/**
		*	Resets the game clock and other things to be initialized before starting the game loop
		*	Must be called before entering the game loop
		*/
		void Start(const std::string & config);


		/**
		 *	Updates the game clock and calls Update on the game World
		 *	Must be called every frame, inside the game loop
		 */
		void Update();

		/**
		 *	Will close off any lose ends before the Game ends
		 *	Must be called after exiting from the game loop
		 *	calls OnDestroy on world
		 */
		void Destroy();

		/**
		 *	Sets the renderer - OpenGL or DirectX
		 */
		void SetRenderer(Renderer* device);

	private:
		void AddParseHelpers();

		Renderer* mRenderer;

		GameClock mGameClock;
		GameTime mGameTime;
		World mWorld;
		AudioManager mAudioManager;
		
		SharedDataTable mSharedData;
		XmlParseMaster mParseMaster;

		XmlParseHelperDefine mDefineParser;
		XmlParseHelperDefineSector mDefineSectorParser;
		XmlParseHelperWorld mWorldParser;
		XmlParseHelperSector mSectorParser;
		XmlParseHelperEntity mEntityParser;
		XmlParseHelperGameObject mGameObjectParser;
		XmlParseHelperAction mActionParser;
		XmlParseHelperTable mTableParser;
		XmlParseHelperPrimitives mPrimitivesParser;
		XmlParseHelperNameValue mNameValueParser;
		XmlParseHelperActionSwitch mSwitchParser;
		XmlParseHelperActionSwitch::XmlParseHelperActionSwitchCase mCaseParser;
		XmlParseHelperActionExpression mExpressionParser;
		XmlParseHelperReaction mReactionParser;
		XmlParseHelperActionEvent mActionEvent;

		XmlParseHelperActionIf mActionIf;
		XmlParseHelperActionIf::XmlParseHelperActionIfCondition mActionIfCondition;
		XmlParseHelperActionIf::XmlParseHelperActionIfThen mActionIfThen;
		XmlParseHelperActionIf::XmlParseHelperActionIfElse mActionIfElse;

		XmlParseHelperActionWhile mActionWhile;
		XmlParseHelperActionWhile::XmlParseHelperActionWhileLoopBody mActionWhileLoop;

		XmlParseHelperBeginPlay mActionBeginPlay;
	
		XmlParseHelperOnDestroy mActionOnDestroy;

		XmlParseHelperSprite mSpriteParser;
		XmlParseHelperPolygon mPolygonParser;
		XmlParseHelperImage mImageParser;
		XmlParseHelperAnimator mAnimatorParser;
		XmlParseHelperAnimationState mAnimationStateParser;
		XmlParseHelperAnimationFrame mAnimationFrameParser;
		XmlParseHelperCircleColliderComponent mCircleColliderComponent;

		EntityFactory mEntityFactory;
		GameObjectFactory mGameObjectFactory;

		ActionListFactory mActionListFactory;
		ActionListSwitchFactory mActionSwitchFactory;
		ActionListSwitch::ActionListSwitchCaseFactory mActionCaseFactory;
		ActionWhileLoopFactory mActionWhileFactory;
		ActionIfThenElseFactory mActionIfFactory;
		ActionExpressionFactory mActionExpressionFactory;
		ActionEventFactory mActionEventFactory;
		ReactionAttributedFactory mReactionFactory;
		ActionCreateEntityFactory mCreateEntityFactory;
		ActionDestroyEntityFactory mDestroyEntityFactory;
		ActionLoadSectorFactory mLoadSectorFactory;

		ImageFactory mImageFactory;
		SpriteRendererFactory mSpriteRendererFactory;
		CircleColliderComponentFactory mCircleColliderComponentFactory;

		ActionCreateEntityFromFileFactory mCreateEntityFromFileFactory;
		AnimatorFactory mAnimatorFactory;
		AnimationStateFactory mAnimationStateFactory;
		AnimationFrameFactory mAnimationFrameFactory;

		//sound 
		ActionLoadMusicFactory actionLoadMusicFactory;
		ActionPlayMusicFactory actionPlayMusicFactory;
		ActionTogglePauseMusicFactory actionTogglePauseMusicFactory;
		ActionStopMusicFactory actionStopMusicFactory;
	};
}
